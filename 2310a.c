#include "game.h"

int main(int argc, char **argv) {
    TokaidoGame *tokaidoGame = initialize_tokaido_game();
    tokaidoGame->myPlayerType = 'A';
    load_player_arguments(argc, argv, tokaidoGame);
    load_path(tokaidoGame);
    start(tokaidoGame);
    return 0;
}
