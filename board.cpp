#include "board.h"
#include <iostream>
#include <algorithm>
#include <map>
#include "Tile.h"
#include "TileBag.h"

Board::Board() : grid(15, std::vector<Tile*>(15, nullptr)) {
    std::cout << "SCRABBLE GAME\n"; 
}
Board::~Board(){
    for (auto& row : grid){
        for (auto& tile : row){
            delete tile;
            tile = nullptr;
        }
    }
}
void Board::displayBoard() const {
    std::cout << "     ";
    for (int col = 1; col < 16; col++) {
        std::cout << (col < 10 ? " " : "") << col << "   ";
    }
    std::cout << "\n";
    for (int row = 0; row < 15; row++) {
        std::cout << "   +";
        for (int col = 0; col < 15; col++)
            std::cout << "----+";
        std::cout << "\n";
        std::cout << (row + 1 < 10 ? " " : "") << (row + 1) << " |";
        for (int col = 0; col < 15; col++) {
            if (grid[row][col] != nullptr) {
                char letter = grid[row][col]->getLetter();
                std::cout << " " << letter << "  |";  
            } else { 
                std::cout << ((row == 7 && col == 7) ? " *  |" : "    |");
            }
        }
        std::cout << "\n";
    }
    std::cout << "   +"; 
    for (int col = 0; col < 15; col++)
        std::cout << "----+";
    std::cout << "\n";
}
bool Board::placeTiles(int row, int col, const std::vector<Tile>& tiles, bool horizontal) {
    if (!isValidPlacement(row, col, tiles, horizontal)) {
        return false;
    }
    for (int i = 0; i < tiles.size(); i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (grid[r][c] == nullptr) {
            grid[r][c] = new Tile(tiles[i]);
        }
    }
    return true;
}
bool Board::isValidPlacement(int row, int col, const std::vector<Tile>& tiles, bool horizontal) const {
    int endRow = row + (horizontal ? 0 : tiles.size() - 1);
    int endCol = col + (horizontal ? tiles.size() - 1 : 0);
    if (endRow >= 15 || endCol >= 15 || row < 0 || col < 0) {
        return false;
    }
    bool isFirstMove = isEmpty();
    bool coversCenter = false;
    bool connectsToExisting = false;
    for (int i = 0; i < tiles.size(); i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (r == 7 && c == 7) {
            coversCenter = true;
        }
        if (grid[r][c] != nullptr) {
            if (grid[r][c]->getLetter() != tiles[i].getLetter()) {
                return false;  
            }
            connectsToExisting = true;
        } else {
            if (!isFirstMove) {
                const int dr[] = {-1, 1, 0, 0};
                const int dc[] = {0, 0, -1, 1};
                for (int j = 0; j < 4; j++) {
                    int nr = r + dr[j];
                    int nc = c + dc[j];
                    if (nr >= 0 && nr < 15 && nc >= 0 && nc < 15 && grid[nr][nc] != nullptr) {
                        connectsToExisting = true;
                        break;
                    }
                }
            }
        }
    }
    if (isFirstMove && !coversCenter) {
        return false;
    }
    if (!isFirstMove && !connectsToExisting) {
        return false;
    }
    bool placesNewTile = false;
    for (int i = 0; i < tiles.size(); i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (grid[r][c] == nullptr) {
            placesNewTile = true;
            break;
        }
    }
    if (!placesNewTile) {
        return false;
    }
    return true;
}
bool Board::hasTileAt(int row, int col) const {
    return row >= 0 && row < 15 && col >= 0 && col < 15 && grid[row][col] != nullptr;
}
bool Board::isConnected(int row, int col, const std::vector<Tile>& tiles, bool horizontal) const {
    for (int i = 0; i < tiles.size(); i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        for (int j = 0; j < 4; j++) {
            int nr = r + dr[j];
            int nc = c + dc[j];
            if (nr >= 0 && nr < 15 && nc >= 0 && nc < 15 && grid[nr][nc] != nullptr) {
                return true;
            }
        }
    }
    return false;
}
Tile* Board::getTileAt(int row, int col) const {
    if (row >= 0 && row < 15 && col >= 0 && col < 15) {
        return grid[row][col];
    }
    return nullptr;
}
bool Board::coversCenter(int row, int col, int wordLength, bool horizontal) const {
    for (int i = 0; i < wordLength; i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (r == 7 && c == 7) {
            return true;
        }
    }
    return false;
}
int Board::calculateWordScore(int row, int col, const std::vector<Tile>& tiles, bool horizontal) const {
    int totalScore = 0;
    int wordMultiplier = 1;
    for (int i = 0; i < tiles.size(); i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (grid[r][c] == nullptr) {
            totalScore += tiles[i].getScore();
        } else {
            totalScore += grid[r][c]->getScore();
        }
    }
    for (int i = 0; i < tiles.size(); i++) {
        int r = row + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (grid[r][c] == nullptr) {
            std::string perpWord = getWordAt(r, c, !horizontal);
            if (perpWord.length() > 1) {
                int perpScore = 0;
                int startR = r, startC = c;
                while (hasTileAt(startR - (horizontal ? 1 : 0), startC - (horizontal ? 0 : 1))) {
                    startR -= (horizontal ? 1 : 0);
                    startC -= (horizontal ? 0 : 1);
                }
                int offsetR = startR, offsetC = startC;
                for (char ch : perpWord) {
                    Tile* t = getTileAt(offsetR, offsetC);
                    perpScore += t ? t->getScore() : 0;
                    offsetR += (horizontal ? 1 : 0);
                    offsetC += (horizontal ? 0 : 1);
                }
                totalScore += perpScore;
            }
        }
    }
    return totalScore * wordMultiplier;
}
std::string Board::getWordAt(int row, int col, bool horizontal) const {
    while (hasTileAt(row - (horizontal ? 0 : 1), col - (horizontal ? 1 : 0))) {
        row -= (horizontal ? 0 : 1);
        col -= (horizontal ? 1 : 0);
    }

    std::string word;
    while (hasTileAt(row, col)) {
        word += getTileAt(row, col)->getLetter();
        row += (horizontal ? 0 : 1);
        col += (horizontal ? 1 : 0);
    }

    return word;
}
bool Board::isEmpty() const {
    for (int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            if (getTileAt(row, col) != nullptr){
                return false;
            }
        }
    }
    return true;
}
//complete
