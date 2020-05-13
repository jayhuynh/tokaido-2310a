//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_ERROR_H
#define TOKAIDO_2310A_ERROR_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Enum Error type which is used to throw corresponding error in the game
 */
typedef enum {
    PLAYER_OK = 0,
    PLAYER_ARGUMENTS = 1,
    PLAYER_COUNT = 2,
    PLAYER_ID = 3,
    PLAYER_PATH = 4,
    PLAYER_EARLY_GAME_OVER = 5,
    PLAYER_COMMUNICATIONS = 6,
    DEALER_OK = 7,
    DEALER_ARGUMENTS = 8,
    DEALER_DECK = 9,
    DEALER_PATH = 10,
    DEALER_STARTING_PROCESS = 11,
    DEALER_COMMUNICATIONS = 12
} Error;

void throw_error(Error type);

#endif //TOKAIDO_2310A_ERROR_H
