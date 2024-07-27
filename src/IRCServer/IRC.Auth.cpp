/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.Auth.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:00:37 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 15:48:55 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

static bool user_exists(const userInfo &user)
{
  return (!user.password.empty() && !user.nickname.empty() && !user.username.empty() && !user.is_authenticated);
}

void IRC::authenticateClient(int cliSock)
{
  userInfo &user = _userInfo[cliSock];

  if (!user_exists(user)) {
		return ;
	}
  
  if (user.password == _psswd)
  {
    user.is_authenticated = true;
		happendToClient(cliSock, " authenticated.", false);
    sendWelcomeMessages(cliSock);
    return ;
  }
  
  std::string reply = SERVER_NAME + FYEL("That's the wrong password Mr.") + FYEL(ITAL(user.nickname)) + FYEL(" ¯\\_(ツ)_/¯\r\n");
	
  send(cliSock, reply.c_str(), reply.size(), 0);
  close_connection(cliSock);
	happendToClient(cliSock, " provided incorrect password.", true);
}

void IRC::handleNickCollision(int cliSock, std::string & nickname)
{
	int suffix = 0;
	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++) 
  {
		if (it->fd == cliSock) 
			continue;
		if (_userInfo[it->fd].nickname == addNumberToStr(nickname, suffix)) { suffix++; }
	}
	if (suffix > 0)
		nickname = addNumberToStr(nickname, suffix);
}

void IRC::sendWelcomeMessages(int cliSock)
{
	userInfo &user = _userInfo[cliSock];

	std::string welcome;
	std::string info;

	welcome = "\n" + SERVER_NAME + BOLD("Welcome to our IRC network!\n\t\t") + "You " + UNDL(user.nickname) + ", are our dear guest today. Make yourself at home :)\n\n";
	info = "\t\tThis network was created on " + ITAL(_creationDate) + ", by " + ITAL(FMAG ("Sofia")) + " and " + ITAL(FGRN("Simon.")) + "\n";

	send(cliSock, welcome.c_str(), welcome.size(), 0);
	send(cliSock, info.c_str(), info.size(), 0);
}
