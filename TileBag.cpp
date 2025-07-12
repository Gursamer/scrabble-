#include "TileBag.h"
#include <algorithm>
#include <stdexcept>

TileBag::TileBag() : rng(std::random_device{}()) {
    initializeTiles();
    shuffle();
}
void TileBag::initializeTiles(){
    std::vector<std::pair<char, std::pair<int, int>>> distribution = {
        {'A', {1, 9}}, {'B', {3, 2}}, {'C', {3, 2}}, {'D', {2, 4}}, {'E', {1, 12}}, {'F', {4, 2}}, {'G', {2, 3}},
        {'H', {4, 2}}, {'I', {1, 9}}, {'J', {8, 1}}, {'K', {5, 1}}, {'L', {1, 4}}, {'M', {3, 2}}, {'N', {1, 6}},
        {'O', {1, 8}}, {'P', {3, 2}}, {'Q', {10, 1}}, {'R', {1, 6}}, {'S', {1, 4}}, {'T', {1, 6}}, {'U', {1, 4}},
        {'V', {4, 2}}, {'W', {4, 2}}, {'X', {8, 1}}, {'Y', {4, 2}}, {'Z', {10, 1}}, {'_', {0, 2}}
    };
    for (const auto& [letter, data] : distribution){
        auto [points, count] = data;
        for(int i = 0; i < count; i++){
            tiles.emplace_back(letter, points, letter == '_');
        }
        }
    }
    void TileBag::shuffle(){
    std::shuffle(tiles.begin(), tiles.end(), rng);
}
Tile TileBag::draw(){
    if (tiles.empty()){
    throw std::runtime_error("No more tiles!");
    }
    Tile tile = tiles.back();
    tiles.pop_back();
    return tile;
}
void TileBag::returnTile(const Tile& tile){
    tiles.push_back(tile);
}
bool TileBag::isEmpty() const{
    return tiles.empty();
}
size_t TileBag::size() const{
    return tiles.size();
}
//complete
