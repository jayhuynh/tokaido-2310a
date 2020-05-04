#include "game.h"

int main(int argc, char **argv) {
    TokaidoGame *tokaidoGame = initialize_tokaido_game();
    tokaidoGame->myPlayerType = 'B';
    load_player_arguments(argc, argv, tokaidoGame);
    load_player_path(tokaidoGame);
    start_player_game(tokaidoGame);
    free_tokaido_game(tokaidoGame);
    return 0;
}
