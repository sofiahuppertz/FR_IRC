/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:44:40 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 14:34:15 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"
#include "Bot.hpp"

Bot::Bot() {
  srand(time(0));
  startNewGame();
}

void Bot::startNewGame() {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      _board[i][j] = ' ';
    }
  }
  _currentPlayer = 'X';
  _gameOver = false;
  _winner = ' ';
}

std::string Bot::makeMove(int row, int col, char player) {
  if (_gameOver || _board[row][col] != ' ') {
    return FRED("Invalid move!");
  }

  _board[row][col] = player;
  if (checkWin(player)) {
    _gameOver = true;
    _winner = player;
    return FGRN("Player " + std::string(1, player) + " wins!");
  }

  if (checkDraw()) {
    _gameOver = true;
    return FYEL("It's a draw!");
  }

  _currentPlayer = (_currentPlayer == 'X') ? 'O' : 'X';
  return FGRN("Move accepted!");
}

std::string Bot::makeRandomMove() {
  if (_gameOver) {
    return FBLU("Game over!");
  }

  std::vector<std::pair<int, int> > availableMoves;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (_board[i][j] == ' ') {
        availableMoves.push_back(std::make_pair(i, j));
      }
    }
  }

  if (availableMoves.empty()) {
    return FRED("No available moves!");
  }

  int randomIndex = rand() % availableMoves.size();
  int row = availableMoves[randomIndex].first;
  int col = availableMoves[randomIndex].second;

  return makeMove(row, col, 'O');
}

std::string Bot::getBoard() const
{
  std::string board;
  
  board += "\t╔═══╦═══╦═══╗\n";
  for (int i = 0; i < 3; ++i) {
    board += "\t║";
    for (int j = 0; j < 3; ++j) {
      board += (_board[i][j] == ' ') ? " _ " : (" " + std::string(1, _board[i][j]) + " ");
      if (j < 2) board += "║";
    }
    board += "║\n";
    if (i < 2) board += "\t╠═══╬═══╬═══╣\n";
  }
  board += "\t╚═══╩═══╩═══╝\n";
  return board;
}

bool Bot::isGameOver() const {
  return _gameOver;
}

std::string Bot::getWinner() const {
  if (_winner != ' ') {
    return FGRN("Player " + std::string(1, _winner) + " wins!");
  }
  return FYEL("No winner");
}

bool Bot::checkWin(char player) const {
  for (int i = 0; i < 3; ++i) {
    if (_board[i][0] == player && _board[i][1] == player && _board[i][2] == player) return true;
    if (_board[0][i] == player && _board[1][i] == player && _board[2][i] == player) return true;
  }
  if (_board[0][0] == player && _board[1][1] == player && _board[2][2] == player) return true;
  if (_board[0][2] == player && _board[1][1] == player && _board[2][0] == player) return true;
  return false;
}

bool Bot::checkDraw() const {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (_board[i][j] == ' ') {
        return false;
      }
    }
  }
  return true;
}

char Bot::getCurrentPlayer() const {
  return _currentPlayer;
}
