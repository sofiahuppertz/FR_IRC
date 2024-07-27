/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:43:44 by oldault           #+#    #+#             */
/*   Updated: 2024/07/26 11:05:03 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

std::string getCurrentDateTime()
{
  std::time_t now = std::time(0);
  std::tm* now_tm = std::localtime(&now);

  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", now_tm);

  return std::string(buffer);
}
