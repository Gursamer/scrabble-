#ifndef TILE_H
#define TILE_H
#include <string>
class Tile{
     private:
        char letter;
        int score;
        bool isBlank;
    public:
        Tile();
        Tile(char letter = ' ', int score = 0, bool blank = false);
        char getLetter() const;
        int getScore() const;
        bool getIsBlank() const;
        void setLetter(char letter);
        void setScore(int score);
        void setIsBlank(bool blank);
        bool isWildCard() const{
            return isBlank;
        }
};
#endif
//complete
