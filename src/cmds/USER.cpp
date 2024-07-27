/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:28:01 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:32:06 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handleUserCommand(int cliSock, std::istringstream & lineStream) 
{
	std::string username, hostname, servername, realname, reply;

	lineStream >> username >> hostname >> servername;
	std::getline(lineStream, realname);
	_userInfo[cliSock].server_addr = servername;

	handleNickCollision(cliSock, username);
	
	if (isValidNickname(username) == false) {
		reply = getServerReply(ERR_ERRONEUSNICKNAME, cliSock) + " " + UNDL(username) + " : Invalid username\r\n";
		return sendFormatPrintRED(cliSock, reply, SERVER);
	}

	_userInfo[cliSock].username = username;
	if (_userInfo[cliSock].is_authenticated == false) {
		authenticateClient(cliSock);
	}
}
