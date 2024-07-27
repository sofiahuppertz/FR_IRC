/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:26:30 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 15:50:49 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include "libraries.hpp"
#include "definitions.hpp"
#include "structs.hpp"
#include "Colors.hpp"
#include "Bot.hpp"

class IRC
{
	public:
		IRC(const std::string &port, const std::string& password);
		~IRC();

		void start();
		
	//*--------- BONUS --------*//
		Bot Bot_;
		void handleGameCommand(int cliSock, const std::string& message);
		void handleFileCommand(int cliSock, const std::string& message);

	private:

		typedef void (IRC::*CommandHandler)(int, std::istringstream&);
		static IRC* _instance;

    const std::string _port;
		const std::string _psswd;
		std::string _banCmds;
		std::string _creationDate;
	
		int _listener;
    std::vector<struct pollfd> _pfds;
		std::map<int, std::string> _clientBuffers;

		std::map<int, userInfo>_userInfo; //? Users DB
		std::map<std::string, channelInfo>_channelInfo; //? Channel DB
		std::map<std::string, CommandHandler> _cmdHandlers; //? Map with all commands and their handlers
		std::string _availableModes;
	
	//*-------Authentication-----*//
		void authenticateClient(int cliSock);
		void handleNickCollision(int cliSock, std::string & nickname);
		void sendWelcomeMessages(int cliSock);
	//*--------------------------*//

	//*-------- GET & SET -------*//
		std::string getCommandPrefix(int cliSock);
		int					getClientSocket(const std::string &nickname);
		int					getClientSocketFromUsername(const std::string &username);
		std::string getServerReply(int numeric, int cliSock);
		std::string getMemberList(const std::string channel);
		bool				isUser(const std::string & str);
		bool				isOperator(std::string nickname, std::string channel);
		bool				isModeValid(std::string mode);
		void				updateUserMode(int cliSock, std::string user, std::string mode);
		void				updateChannelMode(int cliSock, std::string channel, std::string mode, std::string mode_option);
		void 				removeMember(int cliSock, std::string channel);
		bool 				isValidNickname(const std::string& nickname);
		bool 				isValidChannel(const std::string& channel);
	//*---------------------------//


	//*-------Mode command------*//
		void modeSplitter(int cliSock, std::string channel, std::string mode, std::string mode_option);
		void keyModeManager(int cliSock, std::string channel, std::string mode, std::string mode_option);
		void operatorModeManager(int cliSock, std::string channel, std::string mode, std::string mode_option);
		void userLimitModeManager(int cliSock, std::string channel, std::string mode, std::string mode_option);
		void topicLockModeManager(int cliSock, std::string channel, std::string mode);
		void inviteModeManager(int cliSock, std::string channel, std::string mode);
		std::string gatherModes(std::string channel);
	//*------------------------*//


	//*-------Join command------*//
		void sendChannelMemberList(int cliSock, std::string channel);
		void sendChannelTopic(int cliSock, std::string channel);
		void sendJoinMessage(int cliSock, std::string channel);
	//*------------------------*//

	//*------- Output ------*//
		void printResponse(ResponseMode mode, std::string message);
		void sendAndPrintResponse(int cliSock, const std::string &message, ResponseMode mode);
		void sendFormatPrint(int cliSock, std::string message, ResponseMode mode);
		void sendFormatPrintRED(int cliSock, std::string message, ResponseMode mode);
		void happendToClient(int cliSock, const std::string& happened, bool error);
	//*----------------=----*//

		void broadcastMessage(int senderSocket, const std::string& message, const std::string& channel);
		void sendToOtherChannels(int cliSock, std::string reply);

		static void cleanup();
		
	//*----Socket related functions---*//
    int get_listener_socket();
    int monitor_sockets();
    void receive_connection(int cliSock);
    void close_connection(int socket);
	//*--------------------------------------*//

	//*--------------------------Command related functions------------------*//
		void handleCmds(std::string message, int cliSock);
    void handleNickCommand(int cliSock, std::istringstream& lineStream);
    void handleUserCommand(int cliSock, std::istringstream& lineStream);
    void handleJoinCommand(int cliSock, std::istringstream& lineStream);
		void handlePartCommand(int cliSock, std::istringstream& lineStream);
    void handlePrivmsgCommand(int cliSock, std::istringstream& lineStream);
    void handleKickCommand(int cliSock, std::istringstream& lineStream);
    void handleInviteCommand(int cliSock, std::istringstream& lineStream);
    void handleTopicCommand(int cliSock, std::istringstream& lineStream);
    void handleModeCommand(int cliSock, std::istringstream& lineStream);
    void handlePassCommand(int cliSock, std::istringstream& lineStream);
	//*---------------------------------------------------------------------*//

	//*------ Bot -------*//
		void handleStartCommand(int cliSock);
    void handleMoveCommand(int cliSock, std::istringstream& msgStream);
    bool isValidMove(int row, int col);
    void sendErrorMessage(int cliSock, const std::string& errorMsg);
    void processPlayerMove(int cliSock, int row, int col);
    void processBotMove(int cliSock);
    void sendGameOverMessage(int cliSock);
	//*------------------*//
};


#endif // IRCSERVER_HPP
