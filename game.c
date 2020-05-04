//
// Created by Jay Huynh on 4/28/20.
//

#include "game.h"

TokaidoGame *initialize_tokaido_game() {
    TokaidoGame *newTokaidoGame = malloc(sizeof(TokaidoGame) * 1);
    newTokaidoGame->path = NULL;
    newTokaidoGame->deck = NULL;
    newTokaidoGame->deckOffset = 0;
    newTokaidoGame->players = NULL;
    newTokaidoGame->myId = 0;
    newTokaidoGame->myPlayerType = '\0';
    newTokaidoGame->playerCount = 0;
    return newTokaidoGame;
}

void free_tokaido_game(TokaidoGame *tokaidoGame) {
    free_path(tokaidoGame->path);
    free_players(tokaidoGame->players);
    free(tokaidoGame);
}

String *read_path(FILE *stream) {
    String *path = initialize_string();
    read_from_stream(path, stream, PLAYER_COMMUNICATIONS);
    return path;
}

void load_player_arguments(int argc, char **argv, TokaidoGame *tokaidoGame) {
    if (argc != 3) {
        throw_error(PLAYER_ARGUMENTS);
    }

    int playerCount = string_to_int(argv[1], PLAYER_COUNT);
    if (playerCount < 1) {
        throw_error(PLAYER_COUNT);
    } else {
        tokaidoGame->playerCount = playerCount;
        tokaidoGame->players = malloc(sizeof(Player) * playerCount);
    }
    tokaidoGame->players = initialize_players(tokaidoGame->playerCount);

    int playerId = string_to_int(argv[2], PLAYER_ID);
    if (playerId < 0 || playerId >= playerCount) {
        throw_error(PLAYER_ID);
    } else {
        tokaidoGame->myId = playerId;
    }
}

void load_player_path(TokaidoGame *tokaidoGame) {
    write_string_to_stream("^", stdout);
    String *path = read_path(stdin);
    validate_path(path, tokaidoGame, PLAYER_PATH);
}

void validate_path(String *path, TokaidoGame *tokaidoGame, Error pathError) {
    tokaidoGame->path = initialize_path();
    char *siteCount = strtok(path->buffer, ";");
    char *sites = strtok(NULL, ";");

    if (siteCount == NULL || sites == NULL ||
        strlen(siteCount) + strlen(sites) != (path->length - 1)) {
        throw_error(pathError);
    }

    tokaidoGame->path->siteCount = string_to_int(siteCount, pathError);
    if ((tokaidoGame->path->siteCount * 3) != strlen(sites) ||
        tokaidoGame->path->siteCount < 2) {
        throw_error(pathError);
    }
    tokaidoGame->path->sites = initialize_sites(tokaidoGame->path->siteCount);

    for (int i = 0, j = 0; i < strlen(sites); i += 3, j++) {
        load_site(sites[i], sites[i + 1], sites[i + 2],
                  &tokaidoGame->path->sites[j], tokaidoGame->playerCount,
                  pathError);
    }

    if (tokaidoGame->path->sites[0].type != Barrier ||
        tokaidoGame->path->sites[tokaidoGame->path->siteCount - 1].type !=
        Barrier) {
        throw_error(pathError);
    }
    free_string(path);
}

void load_site(char firstCharacter, char secondCharacter, char capacity,
               Site *site, int maxCapacity, Error pathError) {
    SiteType type = Barrier;
    char *label = "";
    if (firstCharacter == 'M' && secondCharacter == 'o') {
        type = Mo;
        label = "Mo";
    } else if (firstCharacter == 'V' && secondCharacter == '1') {
        type = V1;
        label = "V1";
    } else if (firstCharacter == 'V' && secondCharacter == '2') {
        type = V2;
        label = "V2";
    } else if (firstCharacter == 'D' && secondCharacter == 'o') {
        type = Do;
        label = "Do";
    } else if (firstCharacter == 'R' && secondCharacter == 'i') {
        type = Ri;
        label = "Ri";
    } else if (firstCharacter == ':' && secondCharacter == ':') {
        type = Barrier;
        label = "::";
    } else {
        throw_error(pathError);
    }
    site->type = type;
    site->label = label;

    if (type != Barrier) {
        if (!isdigit(capacity) || capacity == '0') {
            throw_error(pathError);
        }
        site->capacity = capacity - '0';
    } else {
        if (capacity != '-') {
            throw_error(pathError);
        }
        site->capacity = maxCapacity;
    }
    site->visitingPlayersId = malloc(sizeof(int) * site->capacity);
    for (int i = 0; i < site->capacity; ++i) {
        site->visitingPlayersId[i] = -1;
    }
}

void start_player_game(TokaidoGame *tokaidoGame) {
    bool endGame = false;
    add_all_player_first_barrier(tokaidoGame);
    render(tokaidoGame, stderr);
    while (!endGame) {
        String *message = read_message();
        endGame = !process(message, tokaidoGame);
        free_string(message);
    }
    render_final_score(tokaidoGame, stderr);
}

void render(TokaidoGame *tokaidoGame, FILE *stream) {
    Path *path = tokaidoGame->path;
    for (int i = 0; i < path->siteCount; ++i) {
        write_string_to_stream(path->sites[i].label, stream);
        write_string_to_stream(" ", stream);
    }
    write_string_to_stream("\n", stream);

    int renderedPlayer = 0;
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        for (int j = 0; j < path->siteCount; ++j) {
            if (i < path->sites[j].capacity) {
                if (path->sites[j].visitingPlayersId[i] != -1) {
                    write_char_to_stream(
                            path->sites[j].visitingPlayersId[i] + '0', stream);
                    write_string_to_stream("  ", stream);
                    renderedPlayer++;
                } else {
                    write_string_to_stream("   ", stream);
                }
            } else {
                write_string_to_stream("   ", stream);
            }
        }
        write_string_to_stream("\n", stream);
        if (renderedPlayer == tokaidoGame->playerCount) {
            break;
        }
    }
}

void add_all_player_first_barrier(TokaidoGame *tokaidoGame) {
    for (int i = tokaidoGame->playerCount - 1; i > -1; i--) {
        add_player(&tokaidoGame->players[i], &tokaidoGame->path->sites[0]);
    }
}

String *read_message() {
    String *message = initialize_string();
    read_from_stream(message, stdin, PLAYER_COMMUNICATIONS);
    return message;
}

bool process(String *message, TokaidoGame *tokaidoGame) {
    if (strcmp(message->buffer, "YT") == 0) {
        int move = -1;
        if (tokaidoGame->myPlayerType == 'A') {
            get_a_move_of_player_type_a(tokaidoGame, &move);
        } else if (tokaidoGame->myPlayerType == 'B') {
            get_a_move_of_player_type_b(tokaidoGame, &move);
        }
        send_back_move(move);
    } else if (strcmp(message->buffer, "EARLY") == 0) {
        throw_error(PLAYER_EARLY_GAME_OVER);
    } else if (strcmp(message->buffer, "DONE") == 0) {
        return false;
    } else if (message->length >= 12 && message->buffer[0] == 'H' &&
               message->buffer[1] == 'A' && message->buffer[2] == 'P') {
        player_make_a_move(message, tokaidoGame);
    } else {
        throw_error(PLAYER_COMMUNICATIONS);
    }
    return true;
}

void get_a_move_of_player_type_a(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    if (me->money > 0) {
        if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame, Do,
                                                            move)) {
            return;
        }
    }

    if (!path->sites[me->currentSite + 1].isFull &&
        path->sites[me->currentSite + 1].type == Mo) {
        *move = me->currentSite + 1;
        return;
    }

    for (int i = me->currentSite + 1; i < path->siteCount; ++i) {
        if (!path->sites[i].isFull &&
            (path->sites[i].type == V1 || path->sites[i].type == V2 ||
             path->sites[i].type == Barrier)) {
            *move = i;
            return;
        }
    }
}

void get_a_move_of_player_type_b(TokaidoGame *tokaidoGame, int *move) {
    if (get_a_move_other_player_later(tokaidoGame, move)) {
        return;
    }
    if (get_a_move_odd_money(tokaidoGame, move)) {
        return;
    }
    if (get_a_move_most_card(tokaidoGame, move)) {
        return;
    }
    if (get_a_move_v2_between(tokaidoGame, move)) {
        return;
    }
    get_a_move_forward(tokaidoGame, move);
}

bool get_a_move_other_player_later(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        if (tokaidoGame->players[i].id != tokaidoGame->myId) {
            if (path->sites[me->currentSite + 1].isFull ||
                tokaidoGame->players[i].currentSite <= me->currentSite) {
                break;
            }
            if (tokaidoGame->players[i].currentSite > me->currentSite &&
                i == tokaidoGame->playerCount - 1) {
                *move = me->currentSite + 1;
                return true;
            }
        }
    }
    return false;
}

bool get_a_move_odd_money(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];

    if (me->money % 2 == 1) {
        if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame, Mo,
                                                            move)) {
            return true;
        }
    }
    return false;
}

bool get_a_move_most_card(TokaidoGame *tokaidoGame, int *move) {
    bool haveMostCard = true;
    bool everyoneHasZeroCard = true;
    Player *me = &tokaidoGame->players[tokaidoGame->myId];

    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        if (tokaidoGame->players[i].id != tokaidoGame->myId) {
            if (count_card(&tokaidoGame->players[i]) > count_card(me)) {
                haveMostCard = false;
            }
            if (count_card(&tokaidoGame->players[i]) != 0) {
                everyoneHasZeroCard = false;
            }
        }
    }

    if (haveMostCard || everyoneHasZeroCard) {
        if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame, Ri,
                                                            move)) {
            return true;
        }
    }

    return false;
}

bool get_a_move_v2_between(TokaidoGame *tokaidoGame, int *move) {
    if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame, V2,
                                                        move)) {
        return true;
    }

    return false;
}

bool get_a_move_forward(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    for (int i = me->currentSite + 1; i < path->siteCount; ++i) {
        if (!path->sites[i].isFull) {
            *move = i;
            return true;
        }
    }

    return false;
}

int get_a_specific_site_between_us_and_next_barrier(TokaidoGame *tokaidoGame,
                                                    SiteType type, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    for (int i = me->currentSite + 1; i < path->siteCount; ++i) {
        if (path->sites[i].type == Barrier) {
            break;
        }
        if (path->sites[i].type == type && !path->sites[i].isFull) {
            *move = i;
            return true;
        }
    }
    return false;
}

void player_make_a_move(String *message, TokaidoGame *tokaidoGame) {
    char *playerIdStringFormat = strtok(message->buffer + 3, ",");
    char *siteStringFormat = strtok(NULL, ",");
    char *pointStringFormat = strtok(NULL, ",");
    char *moneyStringFormat = strtok(NULL, ",");
    char *cardStringFormat = strtok(NULL, ",");

    if (playerIdStringFormat == NULL || siteStringFormat == NULL ||
        pointStringFormat == NULL ||
        moneyStringFormat == NULL || cardStringFormat == NULL) {
        throw_error(PLAYER_COMMUNICATIONS);
    }
    int playerId = string_to_int(playerIdStringFormat, PLAYER_COMMUNICATIONS);
    if (playerId < 0 || playerId >= tokaidoGame->playerCount) {
        throw_error(PLAYER_COMMUNICATIONS);
    }
    int site = string_to_int(siteStringFormat, PLAYER_COMMUNICATIONS);
    if (site < 0 || site >= tokaidoGame->path->siteCount) {
        throw_error(PLAYER_COMMUNICATIONS);
    }
    int point = string_to_int(pointStringFormat, PLAYER_COMMUNICATIONS);
    if (point < 0) {
        throw_error(PLAYER_COMMUNICATIONS);
    }
    int money = string_to_int(moneyStringFormat, PLAYER_COMMUNICATIONS);
    int card = string_to_int(cardStringFormat, PLAYER_COMMUNICATIONS);
    if (card < 0 || card > 5) {
        throw_error(PLAYER_COMMUNICATIONS);
    }
    update_status(playerId, site, point, money, card, tokaidoGame);
}

void update_status(int playerId, int siteIndex, int point, int money, int card,
                   TokaidoGame *tokaidoGame) {
    Player *player = &tokaidoGame->players[playerId];
    remove_player(player, &tokaidoGame->path->sites[player->currentSite]);
    add_player(player, &tokaidoGame->path->sites[siteIndex]);
    player->point += point;
    player->money += money;
    if (card != 0) {
        player->cards[card - 1]++;
    }
    render_player(player, stderr);
    render(tokaidoGame, stderr);
}

void send_back_move(int move) {
    write_string_to_stream("DO", stdout);
    write_int_to_stream(move, stdout);
    write_char_to_stream('\n', stdout);
}

void render_final_score(TokaidoGame *tokaidoGame, FILE *stream) {
    write_string_to_stream("Scores: ", stream);
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        convert_card_to_point(&tokaidoGame->players[i]);
        convert_v1_v2_to_point(&tokaidoGame->players[i]);
        write_int_to_stream(tokaidoGame->players[i].point, stream);
        if (i != tokaidoGame->playerCount - 1) {
            write_char_to_stream(',', stream);
        }
    }
    write_char_to_stream('\n', stream);
}

void load_dealer_arguments(int argc, char **argv, TokaidoGame *tokaidoGame) {
    if (argc < 4) {
        throw_error(DEALER_ARGUMENTS);
    }

    FILE *deckFile = fopen(argv[1], "r");
    if (deckFile == NULL || ferror(deckFile)) {
        throw_error(DEALER_DECK);
    } else {
        String *deck = read_deck(deckFile);
        validate_deck(deck, tokaidoGame, DEALER_DECK);
    }

    FILE *pathFile = fopen(argv[2], "r");
    if (pathFile == NULL || ferror(pathFile)) {
        throw_error(DEALER_PATH);
    } else {
        String *path = read_path(pathFile);
        validate_path(path, tokaidoGame, DEALER_PATH);
    }
}

String *read_deck(FILE *stream) {
    String *deck = initialize_string();
    read_from_stream(deck, stream, DEALER_DECK);
    return deck;
}

void validate_deck(String *deck, TokaidoGame *tokaidoGame, Error deckError) {
    int cardCount = 0;
    cardCount = (int) strtol(deck->buffer, &tokaidoGame->deck, 10);

    if (cardCount != strlen(tokaidoGame->deck) ||
        strcmp(deck->buffer, "") == 0 ||
        cardCount < 4) {
        throw_error(deckError);
    }

    for (int i = 0; i < cardCount; ++i) {
        if (tokaidoGame->deck[i] < 'A' || tokaidoGame->deck[i] > 'C') {
            throw_error(deckError);
        }
    }

    free_string(deck);
}

void start_player_process(int argc, char **argv, TokaidoGame *tokaidoGame) {
    tokaidoGame->playerCount = argc - 3;
    tokaidoGame->players = initialize_players(tokaidoGame->playerCount);


}
