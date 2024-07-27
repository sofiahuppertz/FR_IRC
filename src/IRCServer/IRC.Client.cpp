/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.Client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 07:32:22 by sofia             #+#    #+#             */
/*   Updated: 2024/07/27 16:47:57 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::receive_connection(int clientFd)
{
  if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) 
    exit(EXIT_FAILURE);

  char buf[MAXDATASIZE];
	int nbytes = recv(clientFd, buf, sizeof buf, 0);

  if  (nbytes <= 0)
  {
    if (nbytes < 0)
      std::cerr << BRED("Failed to receive data: ") 
                << strerror(errno) << std::endl;
    close_connection(clientFd);
  }
  else 
  {
    buf[nbytes] = '\0';
    _clientBuffers[clientFd].append(buf, nbytes);
    
    // Make sure there is no data loss.
    size_t pos;
    while ((pos = _clientBuffers[clientFd].find("\n")) != std::string::npos)
    {
      std::string message = _clientBuffers[clientFd].substr(0, pos + 1);
      handleCmds(_clientBuffers[clientFd], clientFd);
      _clientBuffers[clientFd].erase(0, pos + 1);
    }
  }
}

void IRC::handleCmds(std::string message, int cliSock)
{
	if (message.empty()) 
    return;

	while (!message.empty() && (message[message.size() - 1] == '\r' || message[message.size() - 1] == '\n'))
    message.erase(message.size() - 1);

	printResponse(CLIENT, message);

	std::istringstream cmdAndArg(message);
	std::string cmd;
	cmdAndArg >> cmd;

  if (_cmdHandlers.find(cmd) != _cmdHandlers.end() 
    && _banCmds.find(cmd) == _banCmds.npos)
  { 
    (this->*_cmdHandlers[cmd])(cliSock, cmdAndArg);
  } 
  else
  {
    std::string cmdNotFound = SERVER_NAME + FRED("Command: ") + FRED(ITAL(cmd)) + FRED(" not found...") + "\r\n";
    sendAndPrintResponse(cliSock, cmdNotFound, SERVER);
  }
}

void IRC::close_connection(int cliSock)
{
  std::vector<std::string> channels = _userInfo[cliSock].channels;

  for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
    removeMember(cliSock, *it);
  }
  _userInfo.erase(cliSock);
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); ++it) 
  {
    if (it->fd == cliSock) {
      _pfds.erase(it);
      break;
    }
  }
  close(cliSock);
  happendToClient(cliSock, " disconnected.", true);
}
