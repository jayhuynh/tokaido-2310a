//
// Created by Jay Huynh on 4/28/20.
//

#include "validator.h"

bool are_arguments_valid(int argc, char **argv) {
    if (argc != 2) {
        throw_error(BAD_ARGUMENTS);
    }

    char *error = "";
    int playerCount = (int) strtol(argv[1], &error, 10);
    if (strcmp(error, "") != 0 || playerCount < 1) {
        throw_error(BAD_PLAYER_COUNT);
    }

    int playerId = (int) strtol(argv[2], &error, 10);
    if (strcmp(error, "") != 0 || playerId < 0 || playerId >= playerCount) {
        throw_error(BAD_PLAYER_ID);
    }

    return true;
}


/**
 * Print error message to stderr and exit the program base on error type
 *
 * @param type
 */
void throw_error(Error type) {
    const char *message = "";
    switch (type) {
        case BAD_ARGUMENTS:
            message = "Usage: player pcount ID";
            break;
        case BAD_PLAYER_COUNT:
            message = "Invalid player count";
            break;
        case BAD_PLAYER_ID:
            message = "Invalid ID";
            break;
        case BAD_PATH:
            message = "Invalid path";
            break;
        case EARLY_GAME_OVER:
            message = "Early game over";
            break;
        case BAD_COMMUNICATIONS:
            message = "Communications error";
            break;
    }
    fprintf(stderr, "%s\n", message);
    exit(type);
}