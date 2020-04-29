#include "validator.h"
#include "game.h"

int main(int argc, char **argv) {
    TokaidoGame *tokaidoGame = initialize_tokaido_game();
    validate_arguments(argc, argv, tokaidoGame);
    String *path = read_path();
    validate_path(path, tokaidoGame);
    return 0;
}
