#include "game.h"

int main(int argc, char **argv) {
    TokaidoGame *tokaidoGame = initialize_tokaido_game();
    load_dealer_arguments(argc, argv, tokaidoGame);
    start_player_process(argc, argv, tokaidoGame);
    start_dealer_game(tokaidoGame);
    free_tokaido_game(tokaidoGame);
    return 0;
}
