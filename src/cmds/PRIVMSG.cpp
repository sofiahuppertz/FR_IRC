/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:30:56 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:31:16 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handlePrivmsgCommand(int cliSock, std::istringstream &lineStream)
{
  std::string target;
  lineStream >> target;
  std::string privmsg;
  getline(lineStream, privmsg);
  std::string reply;

  std::string msg = getCommandPrefix(cliSock) + "PRIVMSG " + UNDL(target) + " " + ITAL(privmsg) + "\r\n";

  if (target[0] == '#') {
    if (!isValidChannel(target)) {
      reply = getServerReply(ERR_NOSUCHCHANNEL, cliSock) + " " + UNDL(target) + " : No such channel\r\n";
      return sendFormatPrintRED(cliSock, reply, SERVER);
    }
    broadcastMessage(cliSock, msg, target);
  }
  else {
    if (target == "GameBot") {
      return handleGameCommand(cliSock, privmsg);
    } else if (target == "FILE") {
      return handleFileCommand(cliSock, privmsg.substr(1));
    }
    int targetSocket = getClientSocket(target);
    if (targetSocket == -1 || send(targetSocket, msg.c_str(), msg.size(), 0) == -1) {
      reply = getServerReply(ERR_NOSUCHNICK, cliSock) + " " + UNDL(target) + " : No user with that nickname\r\n";
      sendFormatPrintRED(cliSock, reply, SERVER);
    }
  }
  printResponse(SERVER, msg);
}