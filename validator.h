//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_VALIDATOR_H
#define TOKAIDO_2310A_VALIDATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

typedef enum {
    OK = 0,
    BAD_ARGUMENTS = 1,
    BAD_PLAYER_COUNT = 2,
    BAD_PLAYER_ID = 3,
    BAD_PATH = 4,
    EARLY_GAME_OVER = 5,
    BAD_COMMUNICATIONS = 6
} Error;

void validate_arguments(int argc, char **argv, TokaidoGame*tokaidoGame);
void validate_path(String *path, TokaidoGame* tokaidoGame);
void throw_error(Error type);

#endif //TOKAIDO_2310A_VALIDATOR_H
