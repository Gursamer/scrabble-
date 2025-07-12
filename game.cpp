#include "game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>

Game::Game() : currentPlayerIndex(0), gameOver(false) {}
void Game::loadDictionary(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Could not open dictionary file.");
    }
    std::string word;
    while (infile >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary.insert(word);
    }
}
void Game::setupPlayers() {
    int numPlayers;
    std::cout << "Enter number of players (2-4): ";
    std::cin >> numPlayers;
    if (numPlayers < 2 || numPlayers > 4) {
        throw std::invalid_argument("Invalid number of players.");
    }
    for (int i = 0; i < numPlayers; i++) {
        std::string name;
        std::cout << "Enter name for player " << (i + 1) << ": ";
        std::cin >> name;
        players.emplace_back(name);
        players.back().drawTiles(tileBag, 7);
    }
}
void Game::play() {
    while (!gameOver) {
        playerTurn(players[currentPlayerIndex]);
        if (scoreThisTurn == 0){
            turnsWithoutScoring++;
        } else {
            turnsWithoutScoring = 0;
        }
        if (turnsWithoutScoring >= 6) {
            std::cout << "Six turns without scoring. Would you like to end the game? (y/n): ";
            std::string input;
            std::cin >> input;
            if (input == "y" || input == "Y") {
                gameOver = true;
            } else {
                turnsWithoutScoring = 0;
            }
        }
        if (tileBag.isEmpty() && std::all_of(players.begin(), players.end(), [](const Player& p) {
                return p.getHand().empty();
            })){
                gameOver = true;
                std::cout << "Game over! No tiles i the bag and player has no tiles.\n";
            }
        nextPlayer();
    }
    endGame();
}
void Game::nextPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}
void Game::playerTurn(Player& player) {
    bool validTurn = false;
    std::string errorMessage;
    while (!validTurn && !gameOver) {
        std::cout << "\n" << player.getName() << "'s turn. Score: " << player.getScore() << "\n";
        board.displayBoard();
        if (!errorMessage.empty()) {
            std::cout << "\033[1;31mError: " << errorMessage << "\033[0m\n";
            errorMessage.clear();
        }
        std::cout << "Your tiles: ";
        for (const auto& tile : player.getHand()) {
        std::cout << tile.getLetter() << " ";
    }
    std::cout << "\nOptions: (1) Play (2) Exchange (3) Pass (4) Resign\n";
        std::string command;
        std::cin >> command;
        if (command == "1") {
            int row, col;
            std::string word;
            std::cout << "Enter a word to play: ";
            std::cin >> word;
            std::transform(word.begin(), word.end(), word.begin(), ::toupper);
            std::cout << "You entered: " << word << std::endl;
            if (!validateWord(word)) {
                errorMessage = "The word '" + word + "' is not in the dictionary.";
                continue;
            }
            std::cout << "Enter starting row (1-15) and column (1-15): ";
            if (!(std::cin >> row >> col)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                errorMessage = "Invalid input. Please enter numeric values.";
                continue;
            }
            row -= 1;
            col -= 1;
            if (row < 0 || row >= 15 || col < 0 || col >= 15) {
                errorMessage = "Row and column must be between 1 and 15.";
                continue;
            }
            std::string direction;
            std::cout << "Enter the direction (H for horizontal, V for vertical): ";
            std::cin >> direction;
            bool horizontal = (direction == "H" || direction == "h");

            if ((horizontal && col + word.length() > 15) || (!horizontal && row + word.length() > 15)) {
                errorMessage = "Word doesn't fit on the board.";
                continue;
            }
            bool isFirstMove = board.isEmpty();
            if (isFirstMove) {
                bool coversCenter = false;
                for (size_t i = 0; i < word.length(); i++) {
                    int checkR = row + (horizontal ? 0 : i);
                    int checkC = col + (horizontal ? i : 0);
                    if (checkR == 7 && checkC == 7) {
                        coversCenter = true;
                        break;
                    }
                }
                if (!coversCenter) {
                    errorMessage = "First word must cover the center tile (8,8).";
                    continue;
                }
            }
            std::vector<Tile> tilesToPlace;
            std::vector<char> neededChars;
            int checkRow = row, checkCol = col;
            for (char ch : word) {
                Tile* existing = board.getTileAt(checkRow, checkCol);
                if (existing == nullptr) {
                    neededChars.push_back(ch);
                } else if (existing->getLetter() != ch) {
                    errorMessage = "Tile conflict at (" + std::to_string(checkRow + 1) + "," + std::to_string(checkCol + 1) + ")";
                    break;
                }
                if (horizontal) checkCol++; else checkRow++;
            }
            if (!errorMessage.empty()) continue;
            std::vector<Tile> playerTiles = player.getHand();
            tilesToPlace.clear();
            for (char needed : neededChars) {
                auto it = std::find_if(playerTiles.begin(), playerTiles.end(), [needed](const Tile& t) {
                    return t.getLetter() == needed || t.isWildCard() && t.getLetter() == '_';
                });
                if (it != playerTiles.end()) {
                    Tile placed = *it;
                    if (placed.isWildCard()) {
                        std::cout << "You are using a blank tile. What letter should it represent? ";
                        char substitute;
                        std::cin >> substitute;
                        substitute = toupper(substitute);
                        placed.setLetter(substitute);
                    }
                    tilesToPlace.push_back(placed);
                    playerTiles.erase(it);
                } else {
                    errorMessage = "Missing tile: '" + std::string(1, needed) + "'";
                    break;
                }
            }
            if (!errorMessage.empty()) continue;
            if (!board.isValidPlacement(row, col, tilesToPlace, horizontal)) {
                errorMessage = isFirstMove
                ? "Invalid placement. First word must be connected to the center."
                : "Invalid placement. Word must be connected to existing tiles.";
                continue;
            }
            bool valid = true;
            int index = 0;
            for (char ch : word) {
                int checkR = row + (horizontal ? 0 : index);
                int checkC = col + (horizontal ? index : 0);
                if (board.getTileAt(checkR, checkC) == nullptr) {
                    std::string perpWord = board.getWordAt(checkR, checkC, !horizontal);
                    if (perpWord.length() > 1 && !validateWord(perpWord)) {
                        errorMessage = "Invalid perpendicular word: '" + perpWord + "'";
                        valid = false;
                        break;
                    }
                }
                index++;
            }
            if (!valid) continue;
            int score = board.calculateWordScore(row, col, tilesToPlace, horizontal);
            board.placeTiles(row, col, tilesToPlace, horizontal);
            board.displayBoard();
            player.addScore(score);
            player.playTiles(tilesToPlace);
            player.drawTiles(tileBag, tilesToPlace.size());
            if (tilesToPlace.size() == player.getHand().size()){
                score += 50;
            }
            std::cout << "Word placed! You Scored: " << score << "points.\n";
            std::cout << "Total Score: " << player.getScore() << "\n";
            validTurn = true;
        } else if (command == "2") {
            std::string exchangeInput;
            std::cout << "Enter tiles to exchange: ";
            std::cin >> exchangeInput;
            std::vector<Tile> tilesToExchange;
            for (char c : exchangeInput) {
                tilesToExchange.emplace_back(toupper(c), 0);
            }
            if (player.hasTiles(tilesToExchange)) {
                player.exchangeTiles(tilesToExchange, tileBag);
                std::cout << "Tiles exchanged.\n";
                validTurn = true;
            } else {
                errorMessage = "You don't have those tiles.";
            displayGameState();
            }
        } else if (command == "3") {
            std::cout << player.getName() << " passed.\n";
            validTurn = true;
            displayGameState();
        } else if (command == "4") {
            std::cout << player.getName() << " resigned.\n";
            gameOver = true;
            validTurn = true;
            displayGameState();
        } else {
            errorMessage = "Invalid command. Choose 1-4.";
        }
    }
}
bool Game::validateWord(const std::string& word) const {
    std::string lowerWord = word;
    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
    return dictionary.find(lowerWord) != dictionary.end();
}
void Game::displayGameState() const {
    std::cout << "\nScores:\n";
    for (const auto& player : players) {
        std::cout << player.getName() << ": " << player.getScore() << "\n";
    }
    std::cout << "Tiles remaining: " << tileBag.size() << "\n";
}
void Game::endGame() {
    std::cout << "Game Over! Final Scores:\n";
    int maxScore = 0;
    std::vector<std::string> winners;
    for (const auto& player : players) {
        std::cout << player.getName() << ": " << player.getScore() << "\n";
        if (player.getScore() > maxScore) {
            maxScore = player.getScore();
            winners.clear();
            winners.push_back(player.getName());
        } else if (player.getScore() == maxScore) {
            winners.push_back(player.getName());
        }
    }
    if (winners.size() == 1) {
        std::cout << winners[0] << " wins with " << maxScore << " points!\n";
    } else {
        std::cout << "Tie between: ";
        for (size_t i = 0; i < winners.size(); i++) {
            std::cout << winners[i];
            if(i == winners.size() -2)
            std::cout << " and ";
            else if(i < winners.size() - 2)
            std::cout << ", ";
        }
        std::cout << " with " << maxScore << " points each!\n";
    }
}
void Game::saveGame(const std::string& filename) const {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Could not open file for saving.");
    }
    outfile << players.size() << "\n";
    for (const auto& player : players) {
        outfile << player.getName() << " " << player.getScore() << "\n";
        for (const auto& tile : player.getHand()) {
            outfile << tile.getLetter() << " ";
        }
        outfile << "\n";
    }
    outfile.close();
}
void Game::loadGame(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Could not open file for loading.");
    }
    players.clear();
    gameOver = false;
    currentPlayerIndex = 0;
    int numPlayers;
    infile >> numPlayers;
    infile.ignore();
    for (int i = 0; i < numPlayers; i++) {
        std::string name;
        int score;
        infile >> name >> score;
        players.emplace_back(name);
        players.back().addScore(score);
        std::string hand;
        infile.ignore();
        std::getline(infile, hand);
        std::vector<Tile> tiles;
        for (char c : hand) {
            if (c != ' ') {
                tiles.emplace_back(c, 0);
            }
        }
        players.back().setHand(tiles);
    }
    infile.close();
}
