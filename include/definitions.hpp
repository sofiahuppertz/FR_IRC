/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:16:39 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 12:54:04 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DEFINITIONS_HPP__
#define __DEFINITIONS_HPP__

#include "IRC.hpp"

//-------RPL reply--------//
#define RPL_TOPIC 332
#define RPL_NOTOPIC 331
#define RPL_NAMREPLY 353
#define RPL_ENDOFNAMES 366
#define RPL_CHANNELMODEIS 324
#define RPL_INVITING 341 
//---------------------------//

//-------------ERRORS--------//
#define ERR_UNKNOWNMODE 472
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_KEYSET 467
#define ERR_NEEDMOREPARAMS 461
#define	ERR_BADCHANNELKEY 475
#define ERR_CHANNELISFULL 471
#define ERR_INVITEONLYCHAN 473
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_USERONCHANNEL 407
#define ERR_USERNOTINCHANNEL 441
#define ERR_PASSWDMISMATCH 464
#define ERR_ERRONEUSNICKNAME 432
#define ERR_CANTKICKADMIN 485
//----------------------------//

#define CLIENT_HEADER (BOLD(FMAG("[ Client ]")) + " ►\t")
#define SERVER_HEADER (BOLD(FBLU("[ Server ]")) + " ►\t")
#define BROADCAST_HEADER (BOLD(FBLU("[ ServerBC ]")) + " ►\t")
#define SERVER_NAME (BOLD("⚙️ FT_IRC") + " ►\t")
#define DEFAULT_KICK_REASON (ITAL(FRED("Kicked by an operator")))

#define BAN "PING-PONG-WHOIS"
#define MODES "+i-i+k-k+o-o+t-t+l-l"

//----------LIMITS------------//
#define MIN_PORT 1024
#define MAX_PORT 49151
#define MAXDATASIZE 1024
//----------------------------//


bool wrong_input(int ac, char **av);
void sigint_handler();
std::string getCurrentDateTime();
std::string addNumberToStr(const std::string & nickname, int nb);

#endif // __DEFINITIONS_HPP__