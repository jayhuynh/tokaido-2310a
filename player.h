//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_PLAYER_H
#define TOKAIDO_2310A_PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include "helper.h"

/**
 * Custom Player datatype
 */
typedef struct {
    int id;
    int currentSite;
    int point;
    int v1;
    int v2;
    int money;
    /**
     * Card array. Each player will have 5 card slot
     * Index[0] -> the amount of A card of that player
     * Index[1] -> the amount of B card of that player
     * Index[2] -> the amount of C card of that player
     * Index[3] -> the amount of D card of that player
     * Index[4] -> the amount of E card of that player
     */
    int *cards;
    int readPipe[2]; // Array for creating pipe for player process to read
    int writePipe[2]; // Array for creating pipe for player process to write
    /**
     * Input stream of player process. If the dealer want to send message to
     * player process , it will send to this stream
     */
    FILE *inputStream;
    /**
     * Output stream of player process. If the dealer want to read message from
     * player process , it will read from this stream
     */
    FILE *outputStream;
    pid_t pid; // Process id of player
    bool isDead; // Check if this player process dead or not
} Player;

Player *initialize_players(int playerCount);

void free_players(Player *players);

void render_player(Player *player, FILE *stream);

int count_card(Player *player);

void convert_card_to_point(Player *player);

void convert_v1_v2_to_point(Player *player);

#endif //TOKAIDO_2310A_PLAYER_H
