/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:37 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 12:49:01 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::handlePassCommand(int cliSock, std::istringstream & lineStream)
{
	std::string receivedPassword;
	lineStream >> receivedPassword;

	_userInfo[cliSock].password = receivedPassword;
}
