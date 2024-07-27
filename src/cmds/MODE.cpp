/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:18 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 12:52:34 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::keyModeManager(int cliSock, std::string channel, std::string mode, std::string mode_option)
{
	if (!_channelInfo[channel].key.size() && mode == "+k")
	{
		if (mode_option.size())
		{
			//Update key in db, and brodcast change key message
			_channelInfo[channel].key = mode_option;
			std::string reply = SERVER_NAME;
			reply += "MODE " + channel + " " + mode + " " + mode_option + "\r\n";
			printResponse(SERVER, reply);
			send(cliSock, reply.c_str(), reply.size(), 0);
			broadcastMessage(cliSock, reply, channel);
			return ;
		}
		//needmore params;
		std::string reply = getServerReply(ERR_NEEDMOREPARAMS, cliSock);
		reply += " " + channel + " " + mode + " :Need more params (key)\r\n";
		printResponse(SERVER, reply);
		send(cliSock, reply.c_str(), reply.size(), 0);
		return ;
	} else if (_channelInfo[channel].key.size() && mode == "+k")
	{
		//Error key is already set
		std::string reply = getServerReply(ERR_KEYSET, cliSock);
		reply += " " + channel + " :Key is already set\r\n";
		printResponse(SERVER, reply);
		send(cliSock, reply.c_str(), reply.size(), 0);
		return ;
	} else if (_channelInfo[channel].key.size() && mode == "-k")
	{
		//Remove key from db and broadcast mode change
		_channelInfo[channel].key.erase();
		std::string reply = SERVER_NAME;
		reply += "MODE " + channel + " " + mode + "\r\n";
		printResponse(SERVER, reply);
		send(cliSock, reply.c_str(), reply.size(), 0);
		broadcastMessage(cliSock, reply, channel);
	}
}

void IRC::operatorModeManager(int cliSock, std::string channel, std::string mode, std::string mode_option)
{
	//Errror need more params
	if (!mode_option.size())
	{ 
		std::string reply = getServerReply(ERR_NEEDMOREPARAMS, cliSock);
		reply += " " + channel + " " + mode + " :Need more params (nickname)\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	if (getMemberList(channel).find(mode_option) == std::string::npos)
	{
		//send error no such nick
		std::string reply = getServerReply(ERR_NOSUCHNICK, cliSock);
		reply += " " + mode_option + " :No such nick in channel\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	if (mode == "+o")
	{
		//put in operator
		removeMember(getClientSocket(mode_option), channel);
		_channelInfo[channel].operators.push_back("@" + mode_option);
	} else if (mode == "-o")
	{
		//put in member
		removeMember(getClientSocket(mode_option), channel);
		_channelInfo[channel].members.push_back(mode_option);
	}

	_userInfo[getClientSocket(mode_option)].channels.push_back(channel);
	_channelInfo[channel].userCount += 1;

	std::string reply = SERVER_NAME;
	reply += "MODE " + channel + " " + mode + " " + mode_option + "\r\n";
	printResponse(SERVER, reply);
	send(cliSock, reply.c_str(), reply.size(), 0);
	broadcastMessage(cliSock, reply, channel);
}

void IRC::userLimitModeManager(int cliSock, std::string channel, std::string mode, std::string mode_option)
{
	//Send error nee more params if +l without param
	if (mode == "+l" && !mode_option.size())
	{
		std::string reply = getServerReply(ERR_NEEDMOREPARAMS, cliSock);
		reply += " " + channel + " " + mode + " :Need a key\r\n";
		sendFormatPrint(cliSock, reply, SERVER);
		return ;
	}
	
	//Set or reset userlimit
	if (mode == "+l") {
		_channelInfo[channel].userLimit = std::atoi(mode_option.c_str());
	} else if (mode == "-l") {
		_channelInfo[channel].userLimit = 0;
	}

	std::string reply = SERVER_NAME;
	reply += "MODE " + channel + " " + mode;
	if (mode_option.size())
		reply += " " + mode_option;
	reply += "\r\n";
	sendFormatPrint(cliSock, reply, SERVER);
	broadcastMessage(cliSock, reply, channel);
}

void IRC::topicLockModeManager(int cliSock, std::string channel, std::string mode)
{
	//Update topicIsProtected
	if (mode == "+t") {
		_channelInfo[channel].isTopicProtected = true;
	} else if (mode == "-t") {
		_channelInfo[channel].isTopicProtected = false;
	}

	std::string reply = SERVER_NAME;
	reply += "MODE " + channel + " " + mode + "\r\n";
	sendFormatPrint(cliSock, reply, SERVER);
	broadcastMessage(cliSock, reply, channel);
}

void IRC::inviteModeManager(int cliSock, std::string channel, std::string mode)
{
	if (mode == "+i") {
		_channelInfo[channel].isInviteOnly = true;
	} else if (mode == "-i") {
		_channelInfo[channel].isInviteOnly = false;
	}

	std::string reply = SERVER_NAME;
	reply += "MODE " + channel + " " + mode + "\r\n";
	sendFormatPrint(cliSock, reply, SERVER);
	broadcastMessage(cliSock, reply, channel);
}


std::string IRC::gatherModes(std::string channel)
{
	channelInfo chan = _channelInfo[channel];
	std::string modesNParams;

	if (!chan.userLimit && !chan.isInviteOnly && !chan.isTopicProtected && !chan.key.size()) {
		return modesNParams;
	}
	std::stringstream uLimit;
	uLimit << chan.userLimit;
	modesNParams += " +";
	if (chan.userLimit) modesNParams += "l";
	if (chan.isInviteOnly) modesNParams += "i";
	if (chan.isTopicProtected) modesNParams += "t";
	if (chan.key.size()) modesNParams += "k " + chan.key;
	if (chan.userLimit) modesNParams += " " + uLimit.str();

	return modesNParams;
}

void IRC::modeSplitter(int cliSock, std::string channel, std::string mode, std::string mode_option)
{
	switch (mode[1]) {
		case 'k': keyModeManager(cliSock, channel, mode, mode_option); break;
		case 'o': operatorModeManager(cliSock, channel, mode, mode_option); break;
		case 'l': userLimitModeManager(cliSock, channel, mode, mode_option); break;
		case 't': topicLockModeManager(cliSock, channel, mode); break;
		case 'i': inviteModeManager(cliSock, channel, mode);
	};
}

void IRC::handleModeCommand(int cliSock, std::istringstream & lineStream)
{
	//:NICKNAME!USER@HOST MODE <channel|user> <modes> <modeparams>

	std::string channel_user, mode, mode_option;
	lineStream >> channel_user >> mode >> mode_option;

	if (isUser(channel_user)) updateUserMode(cliSock, channel_user, mode);
	else updateChannelMode(cliSock, channel_user, mode, mode_option);

	return ;
}
