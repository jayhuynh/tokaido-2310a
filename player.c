//
// Created by Jay Huynh on 4/28/20.
//

#include "player.h"

/**
 * Allocate memory for a list of player base on the player count
 * @param playerCount : integer - the amount of player would like to allocate
 * @return : Player pointer to that list of player
 */
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
        newPlayers[i].inputStream = NULL;
        newPlayers[i].outputStream = NULL;
        newPlayers[i].pid = 0;
        for (int j = 0; j < 5; ++j) {
            newPlayers[i].cards[j] = 0;
        }
    }
    return newPlayers;
}

/**
 * Deallocate the player list that we allocated before
 * @param players : Player pointer - the target list of player we want to free
 */
void free_players(Player *players) {
    free(players);
}

/**
 * Print the detail of a specific player into a specific stream
 * @param player : Player pointer - the player we would like to render
 * @param stream : File pointer - the stream we would like to print into
 */
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

/**
 * Loop through the card lis to count the amount of card of a player
 * @param player : Player pointer - the player we would to count
 * @return : integer - the amount of card
 */
int count_card(Player *player) {
    int result = 0;
    for (int i = 0; i < 5; ++i) {
        result += player->cards[i];
    }
    return result;
}

/**
 * Convert all card a player have and add to his point
 *
 * We loop through the card list until the card list is empty
 * In each iteration we will take 1 card from each card type if it still have
 * card inside and form a corresponding cardSet. Then we convert cardSet by
 * a specific case
 *
 * @param player
 */
void convert_card_to_point(Player *player) {
    while (1) {
        int cardSet = 0;
        // Loop through the card list
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

/**
 * Convert v1 and v2 point of a player and add to his point
 * @param player
 */
void convert_v1_v2_to_point(Player *player) {
    player->point += player->v1 + player->v2;
    player->v1 = 0;
    player->v2 = 0;
}
