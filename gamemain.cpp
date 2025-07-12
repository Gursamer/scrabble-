#include "game.h"
int main() {
    Game game;
    game.loadDictionary("words.txt");
    game.setupPlayers();
    game.play();
    return 0;
}
//complete
