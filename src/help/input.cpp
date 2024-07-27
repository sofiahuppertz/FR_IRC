/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:36:56 by oldault           #+#    #+#             */
/*   Updated: 2024/07/26 11:05:03 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

bool wrong_input(int ac, char **av)
{
	// Check if the number of arguments is not equal to 3
	if (ac != 3) 
	{
		std::cerr << "\n\t" << BOLD(BRED(" ⚠️ Input Error Detected ⚠️ ")) << "\n\n\t"
							<< UNDL("Please use the correct syntax: ") << "\n\t\t"
							<< ITAL(FMAG("./ircserv  ")) << ITAL(FGRN("< PORT >  ")) << ITAL(FGRN("< PASSWORD >")) << "\n\n"
							<< std::endl;
		return true;
	}

	const std::string password(av[2]);
	int port = std::atoi(av[1]);

	// Check if the port number is outside the valid range (1024 to 49151)
	if ( port < 1024 || port > 49151) 
	{
		std::cerr << "\n\t" << BOLD(BRED(" ⚠️ Input Error Detected ⚠️")) << "\n\n\t"
							<< UNDL("Please use a valid PORT (>=1024 & <=49151), such as: ") << "\n\t\t"
							<< ITAL(FMAG("./ircserv  ")) << ITAL(FYEL("4221  ")) << ITAL(FGRN("< PASSWORD >")) << "\n\n"
							<< std::endl;
		return true;
	}
	return false;
}
