/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.Is.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:26:51 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/26 19:08:09 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

bool IRC::isOperator(std::string nickname, std::string channel)
{
	std::vector<std::string> operators = _channelInfo[channel].operators;

	return (std::find(operators.begin(), operators.end(), nickname) != operators.end());
}

bool IRC::isModeValid(std::string mode)
{
	return (_availableModes.find(mode) != std::string::npos);
}

bool IRC::isUser(const std::string & str)
{
	std::map<int, userInfo>::iterator it = _userInfo.begin();

	for (; it != _userInfo.end(); it++)
	{
		if (it->second.nickname == str)
			return true;
	}
	return false;
}


