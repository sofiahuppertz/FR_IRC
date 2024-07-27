/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:12 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:29:25 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handleKickCommand(int cliSock, std::istringstream & lineStream)
{
	std::string channel, user, reason;
	lineStream >> channel >> user;
	std::string reply;

	std::string line = lineStream.str();
	size_t pos = line.find(":");
	reason = (pos != std::string::npos) ? line.substr(pos + 1) : "";
	
	if (isValidNickname(user) == false) {
		reply = getServerReply(ERR_NOSUCHNICK, cliSock) + " " + UNDL(user) + " : Invalid username\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	if (isOperator("@" + _userInfo[cliSock].nickname, channel)) {
		// CANT KICK OPERATOR ERROR
		if (isOperator("@" + user, channel)) {
			reply = getServerReply(ERR_CANTKICKADMIN, cliSock) + " " + UNDL(user) + " : Can't kick the channel's operator\r\n";
			return sendFormatPrintRED(cliSock, reply, SERVER);
		}

		reply = getCommandPrefix(cliSock) + "KICK " + channel + " " + UNDL(user) + " ";
		if (reason.size() == 1)
			reply += DEFAULT_KICK_REASON;
		else
			reply += reason;
		reply += "\r\n";
		printResponse(SERVER, reply);
		
		if (send(getClientSocket(user), reply.c_str(), reply.size(), 0) == -1) {
			//nosuchuser;
			reply = getServerReply(ERR_NOSUCHNICK, cliSock) + " " + UNDL(user) + " : No such user\r\n";
			return sendFormatPrintRED(cliSock, reply, SERVER);
		}
		else {
			broadcastMessage(getClientSocket(user), reply, channel);
			removeMember(getClientSocket(user), channel);
		}
	}
	else {
		reply = getServerReply(ERR_CHANOPRIVSNEEDED, cliSock) + " " + UNDL(channel) + " : Action requires an operator\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}
}
