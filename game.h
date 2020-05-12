//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_GAME_H
#define TOKAIDO_2310A_GAME_H

#include "path.h"
#include "helper.h"
#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    Path *path;
    Player *players;
    char *deck;
    int deckOffset;
    int myId;
    char myPlayerType;
    int playerCount;
} TokaidoGame;

TokaidoGame *initialize_tokaido_game();
void free_tokaido_game(TokaidoGame *tokaidoGame);
String *read_path(FILE *stream);
void load_player_arguments(int argc, char **argv, TokaidoGame*tokaidoGame);
void load_player_path(TokaidoGame* tokaidoGame);
void validate_path(String *path, TokaidoGame *tokaidoGame, Error type);
void load_site(char firstCharacter, char secondCharacter, char capacity,
               Site *site, int maxCapacity, Error pathError);
void start_player_game(TokaidoGame *tokaidoGame);
void render(TokaidoGame *tokaidoGame, FILE *stream);
void add_all_player_first_barrier(TokaidoGame *tokaidoGame);
String *read_message(FILE *stream, Error type);
bool player_processor(String *message, TokaidoGame* tokaidoGame);
void get_a_move_of_player_type_a(TokaidoGame *tokaidoGame, int *move);
void get_a_move_of_player_type_b(TokaidoGame *tokaidoGame, int *move);
bool get_a_move_other_player_later(TokaidoGame *tokaidoGame, int *move);
bool get_a_move_odd_money(TokaidoGame *tokaidoGame, int *move);
bool get_a_move_most_card(TokaidoGame *tokaidoGame, int *move);
bool get_a_move_v2_between(TokaidoGame *tokaidoGame, int *move);
bool get_a_move_forward(TokaidoGame *tokaidoGame, int *move);
int get_a_specific_site_between_us_and_next_barrier(TokaidoGame *tokaidoGame,
                                                    SiteType type, int *move);
void player_make_a_move(String *message, TokaidoGame *tokaidoGame);
void update_status(int playerId, int site, int point, int money, int cardType,
                   TokaidoGame *tokaidoGame);
void send_back_move(int move);
void render_final_score(TokaidoGame *tokaidoGame, FILE *stream);
void load_dealer_arguments(int argc, char **argv, TokaidoGame *tokaidoGame);
String *read_deck(FILE *stream);
void validate_deck(String *deck, TokaidoGame *tokaidoGame, Error deckError);
void start_player_process(int argc, char **argv, TokaidoGame *tokaidoGame);
void start_dealer_game(TokaidoGame *tokaidoGame);
Player *get_next_turn_player(TokaidoGame *tokaidoGame);
void send_path_to_all_player(TokaidoGame *tokaidoGame);
void request_a_move(Player *player);
void dealer_processor(String *message, TokaidoGame *tokaidoGame,
                      Player *nextTurnPlayer, bool *earlyEndGame);
bool is_move_valid(int move, TokaidoGame *tokaidoGame, Player *nextTurnPlayer);
void notice_to_all_players(int playerId, int newSite, int additionalPoints,
                           int changeInMoney, int cardType,
                           TokaidoGame *tokaidoGame);
int draw_card_from_deck(TokaidoGame *tokaidoGame, Player *player);
void notice_end_game_to_all_players(TokaidoGame *tokaidoGame);
void notice_early_game_over_to_all_players(TokaidoGame *tokaidoGame);

#endif //TOKAIDO_2310A_GAME_H
