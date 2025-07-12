#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "board.h"
#include "TileBag.h"
#include <iostream>
#include <vector>
#include <unordered_set>

class Game {
    private:
        std::vector<Player> players;
        Board board;
        TileBag tileBag;
        std::unordered_set<std::string> dictionary; 
        int currentPlayerIndex;
        bool gameOver;
        void nextPlayer();
        void playerTurn(Player& player);
        bool validateWord(const std::string& word) const;
        void displayGameState() const;
        void endGame();
        int turnsWithoutScoring = 0;
        int scoreThisTurn = 0;
    public:
        Game();
        void loadDictionary(const std::string &filename);
        void setupPlayers();
        void play();
        void saveGame(const std::string &filename) const;
        void loadGame(const std::string &filename);
};
#endif
//complete
