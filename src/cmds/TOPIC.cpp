/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:52 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:31:44 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handleTopicCommand(int cliSock, std::istringstream & lineStream)
{
	std::string channel, topic;
	lineStream >> channel;
	std::getline(lineStream, topic);
	std::string reply;
	// Get the iterator to the vector of channels for the given client
	std::vector<std::string>& channels = _userInfo[cliSock].channels;

	// Use std::find to search for the channel
	std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channel);

	if (_channelInfo[channel].isTopicProtected && !isOperator("@" + _userInfo[cliSock].nickname, channel)) {
		reply = getServerReply(ERR_CHANOPRIVSNEEDED, cliSock) + " " + UNDL(channel) + " : Not enough privilege\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	if (it != channels.end()) {
		_channelInfo[channel].topic = topic;
		reply = getCommandPrefix(cliSock) + "TOPIC " + UNDL(channel) + ITAL(topic) + "\r\n";
		sendFormatPrint(cliSock, reply, SERVER);
		broadcastMessage(cliSock, reply, channel);
	}
	else {
		reply = getServerReply(ERR_NOSUCHCHANNEL, cliSock) + " " + UNDL(channel) + " : Unexisting channel mentionned\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}
}
