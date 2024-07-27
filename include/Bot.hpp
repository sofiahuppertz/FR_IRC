/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oldault <oldault@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:44:46 by svolodin          #+#    #+#             */
/*   Updated: 2024/07/27 10:59:20 by oldault          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <vector>
#include <string>

class Bot {
  public:
    Bot();
    void startNewGame();
    std::string makeMove(int row, int col, char player);
    std::string makeRandomMove();
    std::string getBoard() const;
    bool isGameOver() const;
    std::string getWinner() const;
    char getCurrentPlayer() const;

  private:
    bool checkWin(char player) const;
    bool checkDraw() const;
    char _board[3][3];
    char _currentPlayer;
    bool _gameOver;
    char _winner;
};

#endif
