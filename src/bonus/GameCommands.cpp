/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameCommands.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 11:19:42 by oldault           #+#    #+#             */
/*   Updated: 2024/07/27 12:54:23 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Bot.hpp"

void IRC::handleGameCommand(int cliSock, const std::string& message) {
  std::istringstream msgStream(message);
  std::string command;
  msgStream >> command;

  // Remove leading ':' from command if present
  if (!command.empty() && command[0] == ':') {
      command = command.substr(1);
  }

  if (command == "start") {
      handleStartCommand(cliSock);
  } else if (command == "move") {
      handleMoveCommand(cliSock, msgStream);
  } else {
      sendErrorMessage(cliSock, "Unknown command.");
  }
}

void IRC::handleStartCommand(int cliSock) {
  Bot_.startNewGame();
  std::string startMessage = getCommandPrefix(cliSock) + "PRIVMSG " + std::to_string(cliSock) + FGRN(" :New Tic Tac Toe game started!\n") + Bot_.getBoard() + "\r\n";
  send(cliSock, startMessage.c_str(), startMessage.length(), 0);
}

void IRC::handleMoveCommand(int cliSock, std::istringstream& msgStream) {
  int row, col;
  msgStream >> row >> col;
  
  if (msgStream.fail() || !isValidMove(row, col)) {
    sendErrorMessage(cliSock, "Invalid move! Please use the format 'move <row> <col>' where row and col are between 0 and 2.");
    return;
  }
  
  processPlayerMove(cliSock, row, col);

  if (!Bot_.isGameOver()) {
    processBotMove(cliSock);
  }
}

bool IRC::isValidMove(int row, int col) {
  return row >= 0 && row <= 2 && col >= 0 && col <= 2;
}

void IRC::sendErrorMessage(int cliSock, const std::string& errorMsg) {
  std::string errMsg = getCommandPrefix(cliSock) + "PRIVMSG " + std::to_string(cliSock) + FRED(" :" + errorMsg + "\r\n");
  send(cliSock, errMsg.c_str(), errMsg.length(), 0);
}

void IRC::processPlayerMove(int cliSock, int row, int col) {
  std::string result = Bot_.makeMove(row, col, 'X');
  std::string boardMessage = getCommandPrefix(cliSock) + "PRIVMSG " + std::to_string(cliSock) + " :" + result + "\n" + Bot_.getBoard() + "\r\n";
  send(cliSock, boardMessage.c_str(), boardMessage.length(), 0);

  if (Bot_.isGameOver()) {
      sendGameOverMessage(cliSock);
  }
}

void IRC::processBotMove(int cliSock) {
  std::string result = Bot_.makeRandomMove();
  std::string boardMessage = getCommandPrefix(cliSock) + "PRIVMSG " + std::to_string(cliSock) + " :" + result + "\n" + Bot_.getBoard() + "\r\n";
  send(cliSock, boardMessage.c_str(), boardMessage.length(), 0);

  if (Bot_.isGameOver()) {
      sendGameOverMessage(cliSock);
  }
}

void IRC::sendGameOverMessage(int cliSock) {
  std::string gameOverMessage = getCommandPrefix(cliSock) + "PRIVMSG " + std::to_string(cliSock) + FBLU(" :Game over! ") + Bot_.getWinner() + "\r\n";
  send(cliSock, gameOverMessage.c_str(), gameOverMessage.length(), 0);
}
