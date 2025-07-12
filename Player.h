#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>
#include "Tile.h"
#include "TileBag.h"
class Player{
    private:
        std::string name;
        int score;
        std::vector<Tile> hand;
    public:
        Player(const std::string &name);
        void drawTiles(TileBag &tileBag, int numTiles);
        bool playTiles(const std::vector<Tile> &tiles);
        void exchangeTiles(const std::vector<Tile> &tiles, TileBag &tileBag);
        void addScore(int points);
        void setHand(const std::vector<Tile>& newHand);
        int getScore() const;
        const std::string &getName() const;
        const std::vector<Tile>& getHand() const;
        bool hasTiles (const std::vector<Tile> &tiles) const;
        std::string getHandLetters() const;
        void setBlankLetter(char letter);
};
#endif
//complete
