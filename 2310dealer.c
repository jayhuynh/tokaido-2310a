#include "game.h"

int main(int argc, char **argv) {
    if (catch_signal(SIGHUP, handler_sighup) == -1) {
        return 0;
    }
    TokaidoGame *tokaidoGame = initialize_tokaido_game();
    load_dealer_arguments(argc, argv, tokaidoGame);
    start_player_process(argc, argv, tokaidoGame);
    start_dealer_game(tokaidoGame);
    free_tokaido_game(tokaidoGame);
    return 0;
}
