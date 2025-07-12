#ifndef TileBag_H
#define TileBag_H
#include <iostream>
#include <vector>
#include <random>
#include "Tile.h"
class TileBag{
    private:
        std::vector<Tile> tiles;
        std::mt19937 rng;
        void initializeTiles();
    public:
        TileBag();
        Tile draw();
        void returnTile(const Tile& tile);
        void shuffle();
        bool isEmpty() const;
        size_t size() const;
        bool canExchnage(int count) const;
};
#endif
//complete
