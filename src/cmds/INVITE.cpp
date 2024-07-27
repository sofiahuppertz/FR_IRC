/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:26:59 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:28:50 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handleInviteCommand(int cliSock, std::istringstream & lineStream)
{
	std::string user, channel;
	lineStream >> user >> channel;
	std::string reply;
	
	if (isValidNickname(user) == false)
	{
		reply = getServerReply(ERR_NOSUCHNICK, cliSock);
		reply += " " + UNDL(user) + " : Invalid username\r\n";
		return sendFormatPrint(cliSock, reply, SERVER);
	}

	if ((isOperator("@" + _userInfo[cliSock].nickname, channel) && _channelInfo[channel].isInviteOnly) || !_channelInfo[channel].isInviteOnly) {

		reply = getCommandPrefix(cliSock);
		reply += "INVITE " + UNDL(user) + " " + channel + "\r\n";

		if (send(getClientSocket(user), reply.c_str(), reply.size(), 0) == -1) {
			//nosuchuser;
			reply = getServerReply(ERR_NOSUCHNICK, cliSock);
			reply += " " + UNDL(user) + " : No such user\r\n";
			return sendFormatPrint(cliSock, reply, SERVER);
		} else {
			_channelInfo[channel].inviteList.push_back(user);
			reply = getServerReply(RPL_INVITING, cliSock);
			reply += " " + UNDL(user) + " " + channel + "\r\n";
			return sendFormatPrint(cliSock, reply, SERVER);
		}
	}
}
