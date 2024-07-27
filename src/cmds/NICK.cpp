/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:26 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:30:06 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
struct ReplaceNick
{
	const std::string& oldNick;
	const std::string& newNick;

	ReplaceNick(const std::string& oldNick, const std::string& newNick) :
		oldNick(oldNick),
		newNick(newNick)
	{}

	void operator()(std::pair<const std::string, channelInfo>& channel) const
	{
		std::vector<std::string>& members = channel.second.members;
		std::vector<std::string>& operators = channel.second.operators;
		std::replace(members.begin(), members.end(), oldNick, newNick);
		std::replace(operators.begin(), operators.end(), "@" + oldNick, "@" + newNick);
	}
};

void IRC::sendToOtherChannels(int cliSock, std::string reply)
{
	std::vector<std::string> userChannels = _userInfo[cliSock].channels;
	std::vector<std::string>::iterator it = userChannels.begin();
	std::vector<std::string> ppl_seen;

	for (; it != userChannels.end(); it++)
	{
		std::vector<std::string> cMember = _channelInfo[*it].members;
		std::vector<std::string> cOperator = _channelInfo[*it].operators;
		std::vector<std::string>::iterator mit = cMember.begin();
		std::vector<std::string>::iterator oit = cOperator.begin();

		for (; mit != cMember.end(); mit++) {
			if (std::find(ppl_seen.begin(), ppl_seen.end(), *mit) == ppl_seen.end() && *mit != _userInfo[cliSock].nickname) {
				printResponse(SERVER, reply);
				send(getClientSocket(*mit), reply.c_str(), reply.size(), 0);
				ppl_seen.push_back(*mit);
			}
		}
		for (; oit != cOperator.end(); oit++) {
			std::string real_username = (*oit).substr(1);
			if (std::find(ppl_seen.begin(), ppl_seen.end(), real_username) == ppl_seen.end() && real_username != _userInfo[cliSock].nickname) {
				printResponse(SERVER, reply);
				send(getClientSocket(real_username), reply.c_str(), reply.size(), 0);
				ppl_seen.push_back(real_username);
			}
		}
	}
}

// Gestion de la commande NICK
void IRC::handleNickCommand(int cliSock, std::istringstream & lineStream) {
	std::string nickname;
	lineStream >> nickname;

	if (isValidNickname(nickname) == false) {
		std::string invalidNickname;
		invalidNickname = getServerReply(ERR_ERRONEUSNICKNAME, cliSock) + " " + UNDL(nickname) + " : Invalid nickname\r\n";
		return sendFormatPrintRED(cliSock, invalidNickname, SERVER);
	}
	handleNickCollision(cliSock, nickname);
	
	if (!_userInfo[cliSock].is_authenticated) {
		_userInfo[cliSock].nickname = nickname;
		return ;
	}

	std::string oldNick = _userInfo[cliSock].nickname;
	std::for_each(_channelInfo.begin(), _channelInfo.end(), ReplaceNick(oldNick, nickname));
	
	std::string reply = getCommandPrefix(cliSock) + "NICK :" + UNDL(nickname) + "\r\n";
	_userInfo[cliSock].nickname = nickname;

	//Send nick Response
	sendFormatPrint(cliSock, reply, SERVER);

	//Broadcast nick change to all channel the users's in
	sendToOtherChannels(cliSock, reply);
}
