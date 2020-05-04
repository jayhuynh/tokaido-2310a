//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_PLAYER_H
#define TOKAIDO_2310A_PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include "helper.h"

typedef struct {
    int id;
    int currentSite;
    int point;
    int v1;
    int v2;
    int money;
    int *cards;
} Player;

Player *initialize_players(int playerCount);
void render_player(Player *player, FILE *stream);
int count_card(Player *player);
void convert_card_to_point(Player *player);
void convert_v1_v2_to_point(Player *player);

#endif //TOKAIDO_2310A_PLAYER_H
