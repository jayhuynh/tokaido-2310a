//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_GAME_H
#define TOKAIDO_2310A_GAME_H

#include "path.h"
#include "helper.h"
#include "path.h"

typedef struct {
    Path *path;
    Player *players;
    char *deck;
    int myId;
    int playerCount;
} TokaidoGame;

TokaidoGame *initialize_tokaido_game();
String *read_path();

#endif //TOKAIDO_2310A_GAME_H
