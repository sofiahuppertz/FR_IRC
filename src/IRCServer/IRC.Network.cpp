/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.Network.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 07:26:05 by sofia             #+#    #+#             */
/*   Updated: 2024/07/27 16:06:36 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

static int create_and_bind_socket(struct addrinfo *ai, int &sockfd) 
{
  struct addrinfo *p;
  int yes = 1;
  
  // Loop through all the results and bind to the first we can.
  for (p = ai; p != NULL; p = p->ai_next)
  {
      if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        continue;

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
      {
        close(sockfd);
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
      }
      
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
      {
        close(sockfd);
        continue;
      }
      break; // Successfully created and bound the socket.
  }
  freeaddrinfo(ai); // All done with this structure.
  
  if (p == NULL) 
    return -1;

  return 0; 
}

int IRC::get_listener_socket() 
{
  int listener;
  int rv;
  
  struct addrinfo hints, *servinfo;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = AI_PASSIVE; // Use my IP
  if ((rv = getaddrinfo(NULL, _port.c_str(), &hints, &servinfo)) != 0) 
  {
    std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
    exit(EXIT_FAILURE);
  }
  
  if (create_and_bind_socket(servinfo, listener) == -1)
    return -1;
  
  if (listen(listener, 10) == -1) {
      std::cerr << "Listen: " << strerror(errno) << std::endl;
      close(listener); // ?
      return -1;
  }

  std::cout << "\n\t" << FGRN(BOLD("Server is up!")) 
            << "\n\t" << ITAL("Listening on port: ") 
            << BGRN(" ") << ITAL(BGRN(_port)) 
            << BGRN(" ") << "\n\n" << std::endl;
  
  return listener;
}

static void add_to_pollfds(std::vector<struct pollfd> &pfds, int newfd)
{
  struct pollfd newpfd = {};
  newpfd.fd = newfd;
  newpfd.events = POLLIN;
  pfds.push_back(newpfd);
}

int IRC::monitor_sockets()
{
  int newfd;
  struct sockaddr_storage remoteaddr;
  socklen_t addrlen;
  
  _pfds.reserve(5);
  add_to_pollfds(_pfds, _listener);
  
  while (1)
  {
    int poll_count = poll(_pfds.data(), _pfds.size(), -1);
    
    if (poll_count == -1) {
        std::cerr << "Poll failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < _pfds.size(); i++)
    {
      if (!(_pfds[i].revents & POLLIN)) 
        continue;

      if (_pfds[i].fd == _listener) // Accept new conexion
      {
        addrlen = sizeof remoteaddr;
        newfd = accept(_listener, (struct sockaddr*)&remoteaddr, &addrlen);
        
        if (newfd == -1)
          std::cerr << "Failed to accept client: " << strerror(errno) << std::endl;
        else 
        {
          add_to_pollfds(_pfds, newfd);
          _userInfo[newfd].is_authenticated = false;
          happendToClient(newfd, " connected!", false);
        }
      } 
      else  // Handle existing conexion
        receive_connection(_pfds[i].fd);
    }
  }
}