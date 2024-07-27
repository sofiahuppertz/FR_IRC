/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 10:24:53 by sofia             #+#    #+#             */
/*   Updated: 2024/07/27 11:43:29 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void sig_handler(int signal_num)
{
  std::cout << "\r\033[K";
  std::cout << "\t" << FBLU("Signal catched.🎣 Exiting gracefully.🕊️") << std::endl;
	exit (signal_num);
}

void sigint_handler()
{
  struct sigaction sa;
  
  std::memset(&sa, 0, sizeof(sa));

  // Set the signal handler function
  sa.sa_handler = sig_handler;

  // Set the flag to restart functions if interrupted by handler
  sa.sa_flags = SA_RESTART;

  // Set up the SIGINT handler
  if (sigaction(SIGINT, &sa, NULL) != 0)
  {
    std::cerr << "\n\t" <<
      BRED("Error setting up signal handler: ") <<
      FRED(std::strerror(errno)) << 
    std::endl;
  }

}
