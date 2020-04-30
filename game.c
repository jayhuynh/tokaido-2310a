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
    write_string_to_stream("^", stdout);
    String *path = initialize_string();
    read_from_stream(path, stdin);
    return path;
}

void load_arguments(int argc, char **argv, TokaidoGame *tokaidoGame) {
    if (argc != 3) {
        throw_error(BAD_ARGUMENTS);
    }

    int playerCount = string_to_int(argv[1], BAD_PLAYER_COUNT);
    if (playerCount < 1) {
        throw_error(BAD_PLAYER_COUNT);
    } else {
        tokaidoGame->playerCount = playerCount;
        tokaidoGame->players = malloc(sizeof(Player) * playerCount);
    }
    tokaidoGame->players = initialize_players(tokaidoGame->playerCount);

    int playerId = string_to_int(argv[2], BAD_PLAYER_ID);
    if (playerId < 0 || playerId >= playerCount) {
        throw_error(BAD_PLAYER_ID);
    } else {
        tokaidoGame->myId = playerId;
    }
}

void load_path(TokaidoGame *tokaidoGame) {
    String *path = read_path();
    tokaidoGame->path = initialize_path();
    char *siteCount = strtok(path->buffer, ";");
    char *sites = strtok(NULL, ";");

    if (siteCount == NULL || sites == NULL ||
        strlen(siteCount) + strlen(sites) != (path->length - 1)) {
        throw_error(BAD_PATH);
    }

    tokaidoGame->path->siteCount = string_to_int(siteCount, BAD_PATH);
    if ((tokaidoGame->path->siteCount * 3) != strlen(sites)) {
        throw_error(BAD_PATH);
    }
    tokaidoGame->path->sites = initialize_sites(tokaidoGame->path->siteCount);

    for (int i = 0, j = 0; i < strlen(sites); i += 3, j++) {
        load_site(sites[i], sites[i + 1], sites[i + 2],
                  &tokaidoGame->path->sites[j], tokaidoGame->playerCount);
    }

    if (tokaidoGame->path->sites[0].type != Barrier ||
        tokaidoGame->path->sites[tokaidoGame->path->siteCount - 1].type !=
        Barrier) {
        throw_error(BAD_PATH);
    }
    free_string(path);
}

void load_site(char firstCharacter, char secondCharacter, char capacity,
               Site *site, int maxCapacity) {
    SiteType type;
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
        label = "V3";
    } else if (firstCharacter == 'R' && secondCharacter == 'i') {
        type = Ri;
        label = "Ri";
    } else if (firstCharacter == ':' && secondCharacter == ':') {
        type = Barrier;
        label = "::";
    } else {
        throw_error(BAD_PATH);
    }
    site->type = type;
    site->label = label;

    if (type != Barrier) {
        if (!isdigit(capacity) || capacity == '0') {
            throw_error(BAD_PATH);
        }
        site->capacity = capacity - '0';
    } else {
        if (capacity != '-') {
            throw_error(BAD_PATH);
        }
        site->capacity = maxCapacity;
    }
    site->visitingPlayersId = malloc(sizeof(int) * site->capacity);
    for (int i = 0; i < site->capacity; ++i) {
        site->visitingPlayersId[i] = -1;
    }
}

void start(TokaidoGame *tokaidoGame) {
    bool endGame = false;
    add_all_player_first_barrier(tokaidoGame);
    render(tokaidoGame, stderr);
    while (!endGame) {
        String *message = read_message();
        endGame = !process(message, tokaidoGame);
        free_string(message);
    }
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
    read_from_stream(message, stdin);
    return message;
}

bool process(String *message, TokaidoGame *tokaidoGame) {
    if (strcmp(message->buffer, "YT") == 0) {
        send_back_a_move(tokaidoGame);
    } else if (strcmp(message->buffer, "EARLY") == 0) {
        throw_error(EARLY_GAME_OVER);
    } else if (strcmp(message->buffer, "DONE") == 0) {
        return false;
    } else if (message->length >= 12 && message->buffer[0] == 'H' &&
               message->buffer[1] == 'A' && message->buffer[2] == 'P') {
        player_make_a_move(message, tokaidoGame);
    } else {
        throw_error(BAD_COMMUNICATIONS);
    }
    return true;
}

void send_back_a_move(TokaidoGame *tokaidoGame) {

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
        throw_error(BAD_COMMUNICATIONS);
    }
    int playerId = string_to_int(playerIdStringFormat, BAD_COMMUNICATIONS);
    if (playerId < 0 || playerId >= tokaidoGame->playerCount) {
        throw_error(BAD_COMMUNICATIONS);
    }
    int site = string_to_int(siteStringFormat, BAD_COMMUNICATIONS);
    if (site < 0 || site >= tokaidoGame->path->siteCount) {
        throw_error(BAD_COMMUNICATIONS);
    }
    int point = string_to_int(pointStringFormat, BAD_COMMUNICATIONS);
    if (point < 0) {
        throw_error(BAD_COMMUNICATIONS);
    }
    int money = string_to_int(moneyStringFormat, BAD_COMMUNICATIONS);
    int card = string_to_int(cardStringFormat, BAD_COMMUNICATIONS);
    if (card < 0 || card > 5) {
        throw_error(BAD_COMMUNICATIONS);
    }
    update_status(playerId, site, point, money, card, tokaidoGame);
}

void update_status(int playerId, int site, int point, int money, int card,
                   TokaidoGame *tokaidoGame) {

}