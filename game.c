//
// Created by Jay Huynh on 4/28/20.
//

#include "game.h"

TokaidoGame *initialize_tokaido_game() {
    TokaidoGame *newTokaidoGame = malloc(sizeof(TokaidoGame) * 1);
    newTokaidoGame->path = NULL;
    newTokaidoGame->deck = NULL;
    newTokaidoGame->players = NULL;
    newTokaidoGame->myId = 0;
    newTokaidoGame->playerCount = 0;
    return newTokaidoGame;
}

String *read_path() {
    write_to_stream("^", stdout);
    String *path = initialize_string();
    read_from_stream(path, stdin);
    return path;
}
