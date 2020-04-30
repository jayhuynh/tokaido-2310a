//
// Created by Jay Huynh on 4/28/20.
//

#include "player.h"

Player* initialize_players(int playerCount){
    Player* newPlayers = malloc(sizeof(Player) * playerCount);
    for (int i = 0; i < playerCount; ++i) {
        newPlayers[i].id = i;
    }
    return newPlayers;
}