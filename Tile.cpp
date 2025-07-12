#include "Tile.h"
Tile::Tile() : letter(' '), score(0), isBlank(false) {}
Tile::Tile(char letter, int score, bool blank) : letter(letter), score(score), isBlank(blank) {}
char Tile::getLetter() const {
    return letter;
}
int Tile::getScore() const {
    return score;
}
bool Tile::getIsBlank() const {
    return isBlank;
}
void Tile::setLetter(char letter) {
    this->letter = toupper(letter);
    if(isBlank){
        score = 0;
    }
}
void Tile::setScore(int score) {
    if(!isBlank){
    this->score = score;
    }
}
void Tile::setIsBlank(bool blank) {
    isBlank = blank;
    if(isBlank){
        score = 0;
    }
}
//complete
