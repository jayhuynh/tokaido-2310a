//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_GAME_H
#define TOKAIDO_2310A_GAME_H

#include "path.h"
#include "helper.h"
#include "path.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    Path *path;
    Player *players;
    char *deck;
    int myId;
    int playerCount;
} TokaidoGame;

TokaidoGame *initialize_tokaido_game();
String *read_path();
void load_arguments(int argc, char **argv, TokaidoGame*tokaidoGame);
void load_path(TokaidoGame* tokaidoGame);
void load_site(char firstCharacter, char secondCharacter, char capacity,
               Site *site, int maxCapacity);
void start(TokaidoGame *tokaidoGame);
void render(TokaidoGame *tokaidoGame, FILE *stream);
void add_all_player_first_barrier(TokaidoGame *tokaidoGame);
String *read_message();
bool process(String *message, TokaidoGame* tokaidoGame);
void send_back_a_move(TokaidoGame *tokaidoGame);
void player_make_a_move(String *message, TokaidoGame *tokaidoGame);
void update_status(int playerId, int site, int point, int money, int card,
                   TokaidoGame *tokaidoGame);

#endif //TOKAIDO_2310A_GAME_H
