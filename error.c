//
// Created by Jay Huynh on 4/28/20.
//

#include "error.h"

/**
 * Print error message to stderr and exit the program base on error type
 * Because I put dealer and player error together, I have to get the dealer
 * error type - 7 in order to get the correct exit status number
 *
 * @param Error type: is used to classify the type of error
 */
void throw_error(Error type) {
    const char *message = "";
    switch (type) {
        case PLAYER_OK:
            break;
        case PLAYER_ARGUMENTS:
            message = "Usage: player pcount ID";
            break;
        case PLAYER_COUNT:
            message = "Invalid player count";
            break;
        case PLAYER_ID:
            message = "Invalid ID";
            break;
        case PLAYER_PATH:
            message = "Invalid path";
            break;
        case PLAYER_EARLY_GAME_OVER:
            message = "Early game over";
            break;
        case PLAYER_COMMUNICATIONS:
            message = "Communications error";
            break;
        case DEALER_OK:
            break;
        case DEALER_ARGUMENTS:
            message = "Usage: 2310dealer deck path p1 {p2}";
            break;
        case DEALER_DECK:
            message = "Error reading deck";
            break;
        case DEALER_PATH:
            message = "Error reading path";
            break;
        case DEALER_STARTING_PROCESS:
            message = "Error starting process";
            break;
        case DEALER_COMMUNICATIONS:
            message = "Communications error";
            break;
    }
    fprintf(stderr, "%s\n", message);
    if (type > 6) {
        exit((int) type - 7);
    } else{
        exit(type);
    }
}
