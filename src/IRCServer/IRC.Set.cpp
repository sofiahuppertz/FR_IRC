/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.Set.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:34:07 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 12:52:34 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::updateUserMode(int cliSock, std::string user, std::string mode)
{
	std::string reply = getCommandPrefix(cliSock);
	reply += "MODE " + user + " " + mode + "\r\n";

	printResponse(SERVER, reply);
	send(cliSock, reply.c_str(), reply.size(), 0);
}

void IRC::updateChannelMode(int cliSock, std::string channel, std::string mode, std::string mode_option)
{
	if (!isValidChannel(channel))
	{
		std::string noSuchChannel = getServerReply(ERR_NOSUCHCHANNEL, cliSock);
		noSuchChannel += " " + channel + " :No such channel on ft_irc\r\n";
		printResponse(SERVER, noSuchChannel);
		send(cliSock, noSuchChannel.c_str(), noSuchChannel.size(), 0);
		return ;
	}

	std::string reply;
	//Print current channel modes
	if (!mode.size() && !mode_option.size())
	{
		reply = getServerReply(RPL_CHANNELMODEIS, cliSock) + " " + channel;
		reply += gatherModes(channel) + "\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	//Send error, unknown mode
	if (mode.size() && !isModeValid(mode))
	{
		reply = getServerReply(ERR_UNKNOWNMODE, cliSock);
		reply += " " + mode + " : Mode unknown\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	//Change mode
	if (isOperator("@" + _userInfo[cliSock].nickname, channel)) {
		modeSplitter(cliSock, channel, mode, mode_option);
	}
	else {
		reply = getServerReply(ERR_CHANOPRIVSNEEDED, cliSock) + " " + channel + " : User not operator\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}
}
