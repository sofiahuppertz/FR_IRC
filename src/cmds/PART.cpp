/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:31 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:30:38 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::removeMember(int cliSock, std::string channel) {
	std::vector<std::string> & memberVect = _channelInfo[channel].members;
	std::vector<std::string> & operatorVect = _channelInfo[channel].operators;
	std::vector<std::string>::iterator it;

	if ((it = std::find(memberVect.begin(),memberVect.end(), _userInfo[cliSock].nickname)) != memberVect.end()) {
		memberVect.erase(it);
	} else if ((it = std::find(operatorVect.begin(),operatorVect.end(), "@" + _userInfo[cliSock].nickname)) != operatorVect.end()) {
		operatorVect.erase(it);
	}
	std::vector<std::string> & userChannels = _userInfo[cliSock].channels;
	it = userChannels.begin();
	if ((it = std::find(userChannels.begin(), userChannels.end(), channel)) != userChannels.end()){
		userChannels.erase(it);
		_channelInfo[channel].userCount -= 1;
	}
}

void IRC::handlePartCommand(int cliSock, std::istringstream & lineStream) {

	std::string channel;
	lineStream >> channel;
	std::string reply;
	
	if (isValidChannel(channel) == false) {
		reply = getServerReply(ERR_NOSUCHCHANNEL, cliSock) + " " + UNDL(channel) + " : No such channel\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	// Get the iterator to the vector of channels for the given client
	std::vector<std::string>& channels = _userInfo[cliSock].channels;
	// Use std::find to search for the channel
	std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channel);

	if (it != channels.end()) {
		reply = getCommandPrefix(cliSock) + "PART " + UNDL(channel) + " .\r\n";
		sendFormatPrintRED(cliSock, reply, SERVER);
		broadcastMessage(cliSock, reply, channel);
		channels.erase(it);
		removeMember(cliSock, channel);
  }
	else {
		reply = getServerReply(ERR_USERNOTINCHANNEL,cliSock) + " " + _userInfo[cliSock].nickname + " " + UNDL(channel) + " : You're not in this channel\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}
}
