/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:18:03 by oldault           #+#    #+#             */
/*   Updated: 2024/07/26 13:51:30 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "libraries.hpp"

typedef struct channelInfo {
	std::string topic;
	std::vector<std::string> members;
	std::vector<std::string> operators;
	std::vector<std::string> inviteList;
	std::string key;
	int userCount;
	int userLimit;
	int isTopicProtected;
	int isInviteOnly;

} channelInfo;

typedef struct userInfo {
	bool is_authenticated;
	std::string password;
	std::string nickname;
	std::string username;
	std::string server_addr;
	std::vector<std::string> channels;

} userInfo;

enum ResponseMode {
	CLIENT,
	SERVER,
	BROADCAST,
	NONE
};

#endif // __STRUCTS_H__