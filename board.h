#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "Tile.h"
class Board {
    private:
        std::vector<std::vector<Tile*>> grid;
        std::vector<std::vector<int>> premiumSquares;
        void initializeBoard();
        void initializePremiumSquares();
    public:
        Board();
        ~Board();
        void displayBoard() const;
        bool placeTiles(int row, int col, const std::vector<Tile>& tiles, bool horizontal);
        bool isValidWord(const std::string & word) const;
        bool isValidPlacement(int row, int col, const std::vector<Tile>& tiles, bool horizontal) const;
        bool hasTileAt(int row, int col) const;
        bool isConnected(int row, int col, const std::vector<Tile>& tiles, bool horizontal) const;
        Tile* getTileAt(int row, int col) const;
        bool coversCenter(int row, int col, int wordLength, bool horizontal) const;
        int calculateWordScore(int row, int col, const std::vector<Tile>& tiles, bool horizontal) const;
        std::string getWordAt(int row, int col, bool horizontal) const;
        bool isEmpty() const;
        void applyPremiumEffects(int& letterScore, int& worMultiplier, int row, int col) const;
        bool validateAllWords(int row, int col, const std::vector<Tile>& tiles, bool horizontal, const std::unordered_set<std::string>& dict) const;
};  
#endif
//complete    
