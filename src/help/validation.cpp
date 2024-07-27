/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:38:50 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 06:47:04 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

bool IRC::isValidNickname(const std::string& nickname)
{
  if (nickname.length() < 1 || nickname.length() > 9) {
    return false;
  }
  if (!std::isalpha(nickname[0])) {
    return false;
  }

  std::string validChars = "-[]\\{}^_|";
  for (std::size_t i = 1; i < nickname.length(); ++i)
  {
    char c = nickname[i];
    if (!std::isalnum(c) && validChars.find(c) == std::string::npos) { return false; }
  }
  return true;
}

bool IRC::isValidChannel(const std::string& channel) 
{
  if (channel[0] != '#') { return false; }
  std::string validChars = "-[]\\{}^_|";

  for (std::size_t i = 1; i < channel.length(); ++i)
  {
    char c = channel[i];
    if (!std::isalnum(c) && validChars.find(c) == std::string::npos) { return false; }
  }
  return true;
}
