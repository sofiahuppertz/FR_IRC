/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.LifeCycle.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:42:49 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 15:47:46 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

// Singleton instance.
IRC* IRC::_instance = NULL;

IRC::IRC( const std::string &port, const std::string &password ) :
  _port(port),
  _psswd(password),
  _banCmds(BAN),
  _creationDate(getCurrentDateTime()),
  _listener(-1),
  _availableModes(MODES)
{
  // Add all command handlers to the map.
  _cmdHandlers["NICK"] = &IRC::handleNickCommand;
  _cmdHandlers["USER"] = &IRC::handleUserCommand;
  _cmdHandlers["JOIN"] = &IRC::handleJoinCommand;
  _cmdHandlers["PART"] = &IRC::handlePartCommand;
  _cmdHandlers["PRIVMSG"] = &IRC::handlePrivmsgCommand;
  _cmdHandlers["KICK"] = &IRC::handleKickCommand;
  _cmdHandlers["INVITE"] = &IRC::handleInviteCommand;
  _cmdHandlers["TOPIC"] = &IRC::handleTopicCommand;
  _cmdHandlers["MODE"] = &IRC::handleModeCommand;
  _cmdHandlers["PASS"] = &IRC::handlePassCommand;
  
  // Make instance globally accessible.
  _instance = this;

  // Register cleanup function.
  std::atexit(IRC::cleanup);
}

IRC::~IRC()
{
  // Goodbye message
	std::cout << "\n\t" << FRED(BOLD("Server is now shutting down...")) << "\n\t"
            << ITAL("Thank you for your time!") << "\n" << std::endl;

  // Close the listener socket.
  if (_listener != -1)
    close(_listener);
    
  // Close all client sockets.
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
    close(it->fd); 
}

// Cleanup function.
void IRC::cleanup()
{
	if (_instance)
		delete _instance;
}
