//
// Created by Jay Huynh on 4/28/20.
//

#include "error.h"

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
        case OK:
            break;
    }
    fprintf(stderr, "%s\n", message);
    exit(type);
}