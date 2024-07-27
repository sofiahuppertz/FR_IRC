/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:05 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:28:16 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::sendChannelMemberList(int cliSock, std::string channel)
{
	std::string memberList = getMemberList(channel);
	std::string nameReply = getServerReply(RPL_NAMREPLY, cliSock) + " ";

	nameReply += UNDL(channel) + " :" + memberList + "\r\n";
	sendAndPrintResponse(cliSock, nameReply, SERVER);

	std::string endOfNames = getServerReply(RPL_ENDOFNAMES, cliSock) + " ";
	endOfNames += UNDL(channel) + " : End of member list\r\n";
	send(cliSock, endOfNames.c_str(), endOfNames.size(), 0);
}

void IRC::sendChannelTopic(int cliSock, std::string channel)
{
	std::string topic = _channelInfo[channel].topic;
	std::string reply;

	if (topic.size())
	{
		reply = getServerReply(RPL_TOPIC ,cliSock);
		reply += " " + UNDL(channel) + topic + "\r\n";
		printResponse(SERVER, reply);
		send(cliSock, reply.c_str(), reply.size(), 0);

		return ;
  }
	
	reply = getServerReply(RPL_NOTOPIC ,cliSock);
	reply += " " + UNDL(channel) + " : No topic set\r\n";
	printResponse(SERVER, reply);
	send(cliSock, reply.c_str(), reply.size(), 0);
}


void IRC::sendJoinMessage(int cliSock, std::string channel)
{
	std::string joinMessage = getCommandPrefix(cliSock) + "JOIN " + UNDL(channel) + "\r\n";

	sendFormatPrint(cliSock, joinMessage, SERVER);
	broadcastMessage(cliSock, joinMessage, channel);
}

void IRC::handleJoinCommand(int cliSock, std::istringstream & lineStream)
{
	std::string channel, key;
	lineStream >> channel >> key;
	std::string reply;

	std::vector<std::string> inviteList = _channelInfo[channel].inviteList;

	if (isValidChannel(channel) == false) {
		reply = getServerReply(ERR_NOSUCHCHANNEL, cliSock) + " " + UNDL(channel) + " : No such channel\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}
	
	// Check if the user is already in the channel
	if (std::find(_userInfo[cliSock].channels.begin(), _userInfo[cliSock].channels.end(), channel) != _userInfo[cliSock].channels.end()) {
		reply = getServerReply(ERR_USERONCHANNEL, cliSock) + " " + UNDL(channel) + " : You are already in the channel\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}
	
	//Error invite only
	if (_channelInfo[channel].isInviteOnly && std::find(inviteList.begin(), inviteList.end(), _userInfo[cliSock].nickname) == inviteList.end()) {
		reply = getServerReply(ERR_INVITEONLYCHAN ,cliSock) + " " + UNDL(channel) + " : You're not invited to this channel\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	//Error wrong key
	if (_channelInfo[channel].key.size() && key != _channelInfo[channel].key) {
		reply = getServerReply(ERR_BADCHANNELKEY ,cliSock) + " " + UNDL(channel) + " : Incorrect channel key\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	//Error channel is full
	if (_channelInfo[channel].userLimit != 0  && _channelInfo[channel].userCount >= _channelInfo[channel].userLimit) {
		reply = getServerReply(ERR_CHANNELISFULL,cliSock) + " " + UNDL(channel) + " : Channel is full\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	_userInfo[cliSock].channels.push_back(channel);
	_channelInfo[channel].userCount += 1;

	if (!getMemberList(channel).size())
		_channelInfo[channel].operators.push_back("@" + _userInfo[cliSock].nickname);
	else
		_channelInfo[channel].members.push_back(_userInfo[cliSock].nickname);

	sendJoinMessage(cliSock, channel);
	sendChannelMemberList(cliSock, channel);
	sendChannelTopic(cliSock, channel);
}
