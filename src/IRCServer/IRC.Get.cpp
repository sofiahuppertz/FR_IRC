/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.Get.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:27:58 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 12:54:04 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

std::string IRC::getCommandPrefix(int cliSock)
{
	std::string str;

	str += ":" + _userInfo[cliSock].nickname;
	str += "!" + _userInfo[cliSock].username;
	str += "@" + _userInfo[cliSock].server_addr;
	str += " ";

	return str;
}

int IRC::getClientSocket(const std::string &nickname)
{
	std::map<int, userInfo>::iterator it = _userInfo.begin();
	for (; it != _userInfo.end(); it++) {
		if (it->second.nickname == nickname)
			return it->first;
	}
	return -1;
}

int	IRC::getClientSocketFromUsername(const std::string &username)
{
	std::map<int, userInfo>::iterator it = _userInfo.begin();
	for (; it != _userInfo.end(); it++){
		if (it->second.username == username)
			return it->first;
	}
	return -1;
}

std::string IRC::getServerReply(int numeric, int cliSock)
{
	std::stringstream numeric_stream;
	numeric_stream << numeric;

	std::string numeric_string = numeric_stream.str();
	std::string reply = SERVER_NAME + numeric_string +  " " + _userInfo[cliSock].nickname;

	return reply;
}

std::string IRC::getMemberList(const std::string channel)
{
	std::stringstream members_stream;
	std::vector<std::string> memberVect = _channelInfo[channel].members;
	std::vector<std::string> operatorVect = _channelInfo[channel].operators;

	for (std::vector<std::string>::iterator it = operatorVect.begin(); it != operatorVect.end(); it++)
  {
		members_stream << *it;
		//Initialize an iterator to the next member
		std::vector<std::string>::iterator it_p1 = it;
		std::advance(it_p1, 1);

		//If there is a next member, add spacing
		if (it_p1 != memberVect.end() || !memberVect.size())
			members_stream << " ";
	}

	for (std::vector<std::string>::iterator it = memberVect.begin(); it != memberVect.end(); it++)
  {
		members_stream << *it;
		//Initialize an iterator to the next member
		std::vector<std::string>::iterator it_p1 = it;
		std::advance(it_p1, 1);

		//If there is a next member, add spacing
		if (it_p1 != memberVect.end())
			members_stream << " ";
	}
	return members_stream.str();
}
