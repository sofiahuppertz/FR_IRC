/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:43 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 16:44:02 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::printResponse(ResponseMode mode, std::string message)
{
  switch (mode)
  {
    case CLIENT: std::cout << CLIENT_HEADER << KMAG; break;
    case SERVER:
        message = message.substr(0, message.size() - 2);
        std::cout << SERVER_HEADER << KGRN; break;
    case BROADCAST:
        message = message.substr(0, message.size() - 2);
        std::cout << BROADCAST_HEADER << KGRN; break;
    case NONE: break;
  }
  std::cout << message << RST << std::endl;
}

void IRC::broadcastMessage(int senderSocket, const std::string& message, const std::string& channel)
{
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
  {
    int cliSock = it->fd;
    std::vector<std::string>& channels = _userInfo[cliSock].channels;

    std::vector<std::string>::iterator ch = std::find(channels.begin(), channels.end(), channel);
    if (cliSock != senderSocket && ch != channels.end())
    {
      printResponse(BROADCAST, UNDL("to client " + BOLD(_userInfo[cliSock].nickname)) + " " + message);
      if (send(cliSock, message.c_str(), message.size(), 0) == -1) {
        close_connection(cliSock);
      }
    }
  }
}

void IRC::sendAndPrintResponse(int cliSock, const std::string &message, ResponseMode mode)
{
	printResponse(mode, message);
	send(cliSock, message.c_str(), message.size(), 0);
}

void IRC::sendFormatPrint(int cliSock, std::string message, ResponseMode mode)
{
	printResponse(mode, message);
  message = "► " + ITAL(FGRN(message));
	send(cliSock, message.c_str(), message.size(), 0);
}

void IRC::sendFormatPrintRED(int cliSock, std::string message, ResponseMode mode)
{
	printResponse(mode, message);
  message = "► " + ITAL(FRED(message));
	send(cliSock, message.c_str(), message.size(), 0);
}

void IRC::happendToClient(int cliSock, const std::string& happened, bool error)
{
	if (error) {
		std::cerr <<
      SERVER_HEADER <<
			FRED("Client on socket n°") <<
			FRED(BOLD(cliSock)) <<
			FRED(happened) <<
		std::endl;
	} else {
		std::cout <<
      SERVER_HEADER <<
			FGRN("Client on socket n°") <<
			FGRN(BOLD(cliSock)) <<
			FGRN(happened) <<
		std::endl;
	}
}
