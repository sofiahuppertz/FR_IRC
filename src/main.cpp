/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:28:11 by sofia             #+#    #+#             */
/*   Updated: 2024/07/27 07:45:12 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

int main(int ac, char **av)
{
	IRC* server;

	if (wrong_input(ac, av)) 
		exit(EXIT_FAILURE);
	
	sigint_handler();

	server = new IRC(std::string(av[1]), std::string(av[2]));

	server->start();
	
	delete server;	
}
