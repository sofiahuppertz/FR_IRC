/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:08:17 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 11:36:47 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRC::start()
{
  _listener = get_listener_socket();

  if (_listener == -1) 
  {
    std::cout << "\n\t" << FYEL("Please check you have a valid port number." )<< std::endl;
    exit(EXIT_FAILURE);
  }
  
  monitor_sockets();
}
