//
// Created by Jay Huynh on 4/28/20.
//

#include "player.h"

Player *initialize_players(int playerCount) {
    Player *newPlayers = malloc(sizeof(Player) * playerCount);
    for (int i = 0; i < playerCount; ++i) {
        newPlayers[i].id = i;
        newPlayers[i].currentSite = -1;
        newPlayers[i].point = 0;
        newPlayers[i].v1 = 0;
        newPlayers[i].v2 = 0;
        newPlayers[i].money = 7;
        newPlayers[i].cards = malloc(sizeof(int) * 5);
        for (int j = 0; j < 5; ++j) {
            newPlayers[i].cards[j] = 0;
        }
    }
    return newPlayers;
}

void free_players(Player *players){
    free(players);
}

void render_player(Player *player, FILE *stream) {
    write_string_to_stream("Player ", stream);
    write_int_to_stream(player->id, stream);
    write_string_to_stream(" Money=", stream);
    write_int_to_stream(player->money, stream);
    write_string_to_stream(" V1=", stream);
    write_int_to_stream(player->v1, stream);
    write_string_to_stream(" V2=", stream);
    write_int_to_stream(player->v2, stream);
    write_string_to_stream(" Points=", stream);
    write_int_to_stream(player->point, stream);
    write_string_to_stream(" A=", stream);
    write_int_to_stream(player->cards[0], stream);
    write_string_to_stream(" B=", stream);
    write_int_to_stream(player->cards[1], stream);
    write_string_to_stream(" C=", stream);
    write_int_to_stream(player->cards[2], stream);
    write_string_to_stream(" D=", stream);
    write_int_to_stream(player->cards[3], stream);
    write_string_to_stream(" E=", stream);
    write_int_to_stream(player->cards[4], stream);
    write_string_to_stream("\n", stream);
}

int count_card(Player *player) {
    int result = 0;
    for (int i = 0; i < 5; ++i) {
        result += player->cards[i];
    }
    return result;
}

void convert_card_to_point(Player *player) {
    while (1) {
        int cardSet = 0;
        for (int i = 0; i < 5; ++i) {
            if (player->cards[i] != 0) {
                cardSet++;
                player->cards[i]--;
            }
        }
        switch (cardSet) {
            case 5:
                player->point += 10;
                break;
            case 4:
                player->point += 7;
                break;
            case 3:
                player->point += 5;
                break;
            case 2:
                player->point += 3;
                break;
            case 1:
                player->point += 1;
                break;
        }
        if (cardSet == 0) {
            break;
        }
    }
}

void convert_v1_v2_to_point(Player *player) {
    player->point += player->v1 + player->v2;
    player->v1 = 0;
    player->v2 = 0;
}