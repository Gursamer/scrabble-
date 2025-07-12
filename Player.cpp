#include "Player.h"
#include "TileBag.h"
#include <algorithm>

Player::Player(const std::string& name) : name(name), score(0) {}

void Player::drawTiles(TileBag& bag, int count) {
    while (hand.size() < 7 && count-- > 0 && !bag.isEmpty()) {
        hand.push_back(bag.draw());
    }
}

bool Player::playTiles(const std::vector<Tile>& tiles) {
    std::vector<Tile> tempHand = hand;
    for (const auto& tile : tiles) {
        auto it = std::find_if(tempHand.begin(), tempHand.end(),
            [&tile](const Tile& t) {
                return t.getLetter() == tile.getLetter() || 
                      (t.isWildCard() && tile.isWildCard());
            });
        
        if (it == tempHand.end()) {
            return false;
        }
        tempHand.erase(it);
    }
    hand = tempHand;
    return true;
}

void Player::exchangeTiles(const std::vector<Tile>& tiles, TileBag& bag) {
    if(bag.size()  < tiles.size()){
        throw std::runtime_error("Not enough tiles in the bag to exchange.");
    }
    if (!hasTiles(tiles)) {
        throw std::runtime_error("Player does not have the specified tiles.");
    }
    for (const auto& tile : tiles) {
        bag.returnTile(tile);
    }
    playTiles(tiles);
    drawTiles(bag, tiles.size());
}

void Player::addScore(int points) {
    score += points;
}

void Player::setHand(const std::vector<Tile>& newHand) {
    hand = newHand;
}

const std::string& Player::getName() const {
    return name;
}

int Player::getScore() const {
    return score;
}

const std::vector<Tile>& Player::getHand() const {
    return hand;
}

bool Player::hasTiles(const std::vector<Tile>& tiles) const {
    std::vector<char> temp;
    for (const auto& t : hand) {
        temp.push_back(t.getLetter());
    }
    
    for (const auto& tile : tiles) {
        auto it = std::find(temp.begin(), temp.end(), tile.getLetter());
        if (it == temp.end()) {
            return false;
        }
        temp.erase(it);
    }
    return true;
}

std::string Player::getHandLetters() const {
    std::string letters;
    for (const auto& tile : hand) {
        letters += tile.getLetter();
    }
    return letters;
}
void Player::setBlankLetter(char letter) {
    for (auto& tile : hand) {
        if (tile.isWildCard() && tile.getLetter() == '_') {
            tile.setLetter(letter);
            break;
        }
    }
}
//complete
