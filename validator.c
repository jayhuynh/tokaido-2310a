//
// Created by Jay Huynh on 4/28/20.
//

#include "validator.h"

void validate_arguments(int argc, char **argv, TokaidoGame *tokaidoGame) {
    if (argc != 3) {
        throw_error(BAD_ARGUMENTS);
    }

    char *error = "";
    int playerCount = (int) strtol(argv[1], &error, 10);
    if (strcmp(error, "") != 0 || playerCount < 1) {
        throw_error(BAD_PLAYER_COUNT);
    } else {
        tokaidoGame->playerCount = playerCount;
        tokaidoGame->players = malloc(sizeof(Player) * playerCount);
    }

    int playerId = (int) strtol(argv[2], &error, 10);
    if (strcmp(error, "") != 0 || playerId < 0 || playerId >= playerCount) {
        throw_error(BAD_PLAYER_ID);
    } else {
        tokaidoGame->myId = playerId;
    }
}

void validate_path(String *path, TokaidoGame *tokaidoGame) {
    tokaidoGame->path = initialize_path();
    char *siteCount = strtok(path->buffer, ";");
    char *sites = strtok(NULL, ";");

    if (siteCount == NULL || sites == NULL ||
        strlen(siteCount) + strlen(sites) != (path->length - 1)) {
        throw_error(BAD_PATH);
    }

    char *error = "";
    tokaidoGame->path->siteCount = (int) strtol(siteCount, &error, 10);
    if (strcmp(error, "") != 0 ||
        (tokaidoGame->path->siteCount * 3) != strlen(sites)) {
        throw_error(BAD_PATH);
    }
    tokaidoGame->path->sites = initialize_sites(tokaidoGame->path->siteCount);

    for (int i = 0, j = 0; i < strlen(sites); i += 3, j++) {
        validate_site(sites[i], sites[i + 1], sites[i + 2],
                      &tokaidoGame->path->sites[j], tokaidoGame->playerCount);
    }
}

void validate_site(char firstCharacter, char secondCharacter, char capacity,
                   Site *site, int maxCapacity) {
    SiteType type;
    if (firstCharacter == 'M' && secondCharacter == 'o') {
        type = Mo;
    } else if (firstCharacter == 'V' && secondCharacter == '1') {
        type = V1;
    } else if (firstCharacter == 'V' && secondCharacter == '2') {
        type = V2;
    } else if (firstCharacter == 'D' && secondCharacter == 'o') {
        type = Do;
    } else if (firstCharacter == 'R' && secondCharacter == 'i') {
        type = Ri;
    } else if (firstCharacter == ':' && secondCharacter == ':') {
        type = Barrier;
    } else {
        throw_error(BAD_PATH);
    }
    site->type = type;

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
    site->visitingPlayers = malloc(sizeof(Player) * site->capacity);
}

/**
 * Print error message to stderr and exit the program base on error type
 *
 * @param type
 */
void throw_error(Error type) {
    const char *message = "";
    switch (type) {
        case BAD_ARGUMENTS:
            message = "Usage: player pcount ID";
            break;
        case BAD_PLAYER_COUNT:
            message = "Invalid player count";
            break;
        case BAD_PLAYER_ID:
            message = "Invalid ID";
            break;
        case BAD_PATH:
            message = "Invalid path";
            break;
        case EARLY_GAME_OVER:
            message = "Early game over";
            break;
        case BAD_COMMUNICATIONS:
            message = "Communications error";
            break;
        case OK:
            break;
    }
    fprintf(stderr, "%s\n", message);
    exit(type);
}