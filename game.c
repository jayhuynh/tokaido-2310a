//
// Created by Jay Huynh on 4/28/20.
//

#include "game.h"

/**
 * Allocate memory for a TokaidoGame
 * @return TokaidoGame pointer to the space we allocated
 */
TokaidoGame *initialize_tokaido_game() {
    TokaidoGame *newTokaidoGame = malloc(sizeof(TokaidoGame) * 1);
    newTokaidoGame->path = NULL;
    newTokaidoGame->deck = NULL;
    newTokaidoGame->deckOffset = 0;
    newTokaidoGame->players = NULL;
    newTokaidoGame->myId = -1;
    newTokaidoGame->myPlayerType = '\0';
    newTokaidoGame->playerCount = 0;
    return newTokaidoGame;
}

/**
 * Deallocated a tokaido game and all of its attributes such as path, player
 * list, deck
 * @param tokaidoGame : TokaidoGame pointer - the target tokaido game we would
 * like to deallocate
 */
void free_tokaido_game(TokaidoGame *tokaidoGame) {
    free_path(tokaidoGame->path);
    free_players(tokaidoGame->players);
    free(tokaidoGame->deck);
    free(tokaidoGame);
}

/**
 * Read a path from a specific stream and return a path in String datatype
 * @param stream : File pointer - the stream we would like to read
 * @return : String pointer - the String that contain path
 */
String *read_path(FILE *stream) {
    String *path = initialize_string();
    read_from_stream(path, stream, PLAYER_COMMUNICATIONS);
    return path;
}

/**
 * Load and check the argument for player process
 * The function will throw the corresponding error in case the argument count
 * or one of the argument value is not valid
 *
 * @param argc : integer - argument count
 * @param argv : string array - argument value
 * @param tokaidoGame : the game that we will load the argument into
 */
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

/**
 * Write the special character "^" to stdout to ask for the path
 * Then read the path from it
 * After that we validate the path
 * @param tokaidoGame : the current game we are setting the path
 */
void load_player_path(TokaidoGame *tokaidoGame) {
    write_string_to_stream("^", stdout);
    String *path = read_path(stdin);
    validate_path(path, tokaidoGame, PLAYER_PATH);
}

/**
 * Validate the path then load it into the game
 *
 * @param path
 * @param tokaidoGame
 * @param pathError
 */
void validate_path(String *path, TokaidoGame *tokaidoGame, Error pathError) {
    tokaidoGame->path = initialize_path();
    tokaidoGame->path->stringFormat = path;
    char *temporaryPath = malloc(sizeof(char) * path->length);

    // Use temporaryPath because the function strtok() will change the string
    // we pass into it
    strcpy(temporaryPath, path->buffer);
    //Split the path base on ";"
    char *siteCount = strtok(temporaryPath, ";");
    char *sites = strtok(NULL, ";");

    // After split have to guarantee that total size of all substring have to
    // equal to original string.
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

    // Loop through each sites and load detail to that site
    for (int i = 0, j = 0; i < strlen(sites); i += 3, j++) {
        load_site(sites[i], sites[i + 1], sites[i + 2],
                &tokaidoGame->path->sites[j], tokaidoGame->playerCount,
                pathError);
    }

    // Check if the path must begin and end with SITE_BARRIER "::"
    if (tokaidoGame->path->sites[0].type != SITE_BARRIER ||
            tokaidoGame->path->sites[tokaidoGame->path->siteCount - 1].type !=
            SITE_BARRIER) {
        throw_error(pathError);
    }
    free(temporaryPath);
}

/**
 * Load detail to a site
 *
 * @param firstCharacter : char - the first and second characters are used to
 * classify this site type
 * @param secondCharacter : char - the first and second characters are used to
 * classify this site type
 * @param capacity : integer - the capacity of the site
 * @param site : Site pointer - the site that we will load into
 * @param maxCapacity : the max capacity in this case is the amount of players
 * in the game. If the site is SITE_BARRIER "::" , it will have the max
 * capacity
 * @param pathError : Error enum type - during the load process if there is
 * an error it will throw this error type
 */
void load_site(char firstCharacter, char secondCharacter, char capacity,
        Site *site, int maxCapacity, Error pathError) {
    SiteType type = SITE_BARRIER;
    char *label = "";
    if (firstCharacter == 'M' && secondCharacter == 'o') {
        type = SITE_MO;
        label = "Mo";
    } else if (firstCharacter == 'V' && secondCharacter == '1') {
        type = SITE_V1;
        label = "V1";
    } else if (firstCharacter == 'V' && secondCharacter == '2') {
        type = SITE_V2;
        label = "V2";
    } else if (firstCharacter == 'D' && secondCharacter == 'o') {
        type = SITE_DO;
        label = "Do";
    } else if (firstCharacter == 'R' && secondCharacter == 'i') {
        type = SITE_RI;
        label = "Ri";
    } else if (firstCharacter == ':' && secondCharacter == ':') {
        type = SITE_BARRIER;
        label = "::";
    } else {
        throw_error(pathError);
    }
    site->type = type;
    site->label = label;

    // Set the capacity base on type
    if (type != SITE_BARRIER) {
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

/**
 * The main game iteration of player
 * At first we will add all player to the first barrier and render the game.
 * After that we will go into loop for playing the game.
 * In the loop, player will read the message from the dealer and process
 * that message. If the message said that the game is end then it will break
 * the loop and show the final score of the players.
 *
 * @param tokaidoGame : the target game that we are running
 */
void start_player_game(TokaidoGame *tokaidoGame) {
    bool endGame = false;
    add_all_player_first_barrier(tokaidoGame);
    render(tokaidoGame, stderr);
    while (!endGame) {
        String *message = read_message(stdin, PLAYER_COMMUNICATIONS);
        endGame = !player_processor(message, tokaidoGame);
        free_string(message);
    }
    render_final_score(tokaidoGame, stderr);
}

/**
 * Render the tokaidogame to a specific stream
 *
 * @param tokaidoGame : the target game we would like to render
 * @param stream : the target stream we would like to print into
 */
void render(TokaidoGame *tokaidoGame, FILE *stream) {
    Path *path = tokaidoGame->path;
    // Print the path first
    for (int i = 0; i < path->siteCount; ++i) {
        write_string_to_stream(path->sites[i].label, stream);
        write_string_to_stream(" ", stream);
    }
    write_string_to_stream("\n", stream);

    int renderedPlayer = 0;
    // Print players in each sites of the path
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        for (int j = 0; j < path->siteCount; ++j) {
            if (i < path->sites[j].capacity) {
                if (path->sites[j].visitingPlayersId[i] != -1) {
                    write_int_to_stream(
                            path->sites[j].visitingPlayersId[i], stream);
                    // In case the player's id has 1 digit we need to add
                    // 2 more space in order to align with the path
                    if (path->sites[j].visitingPlayersId[i] < 10) {
                        write_string_to_stream("  ", stream);
                        // In case the player's id has 2 digit we need to add
                        // 1 more space in order to align with the path
                    } else if (path->sites[j].visitingPlayersId[i] < 100) {
                        write_string_to_stream(" ", stream);
                    }
                    renderedPlayer++;
                } else {
                    // In case if a place in that site is empty
                    // we will print 3 space
                    write_string_to_stream("   ", stream);
                }
            } else {
                // If we reach out the capacity of the site, we also print
                // 3 space
                write_string_to_stream("   ", stream);
            }
        }
        write_string_to_stream("\n", stream);
        if (renderedPlayer == tokaidoGame->playerCount) {
            break;
        }
    }
}

/**
 * Add add player to the first barrier in a tokaidogame
 * @param tokaidoGame : the target game we would like to add
 */
void add_all_player_first_barrier(TokaidoGame *tokaidoGame) {
    for (int i = tokaidoGame->playerCount - 1; i > -1; i--) {
        add_player(&tokaidoGame->players[i], &tokaidoGame->path->sites[0]);
    }
}

/**
 * Read a message from a specific stream . In case we have error during the
 * reading process , it will throw the error type that we passed into
 * @param stream : the stream we would like to read
 * @param type : the error we would like to throw in case have error
 * @return : the String contain message that we want to read
 */
String *read_message(FILE *stream, Error type) {
    String *message = initialize_string();
    read_from_stream(message, stream, type);
    return message;
}

/**
 * The processor to process and classify the type of message
 * @param message : the message that we will classify
 * @param tokaidoGame : the current game we are running
 * @return : if the player receive the ending message it will return true or
 * it will return false to notice that the game is not end yet
 */
bool player_processor(String *message, TokaidoGame *tokaidoGame) {
    if (strcmp(message->buffer, "YT") == 0) {
        int move = -1;
        // Get a move base on player type A or B
        if (tokaidoGame->myPlayerType == 'A') {
            get_a_move_of_player_type_a(tokaidoGame, &move);
        } else if (tokaidoGame->myPlayerType == 'B') {
            get_a_move_of_player_type_b(tokaidoGame, &move);
        }
        // Send back move to dealer or we can say the stdout
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

/**
 * Moving strategy for player type A
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign the new move into it
 */
void get_a_move_of_player_type_a(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    // If player have money and there is a SITE_DO site in front of
    if (me->money > 0) {
        if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame,
                SITE_DO, move)) {
            return;
        }
    }

    // If next site is SITE_MO and it is not full
    if (!path->sites[me->currentSite + 1].isFull &&
            path->sites[me->currentSite + 1].type == SITE_MO) {
        *move = me->currentSite + 1;
        return;
    }

    // Pick the closest SITE_V1, SITE_V2 or SITE_BARRIER
    for (int i = me->currentSite + 1; i < path->siteCount; ++i) {
        if (!path->sites[i].isFull &&
                (path->sites[i].type == SITE_V1 ||
                path->sites[i].type == SITE_V2 ||
                path->sites[i].type == SITE_BARRIER)) {
            *move = i;
            return;
        }
    }
}

/**
 * Moving strategy for player type B
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign a new move to it
 */
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

/**
 * Move forward one site if other players are later than us
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign a new move to it
 * @return : if this case is not possible it will return false to continue with
 * the next case
 */
bool get_a_move_other_player_later(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        if (tokaidoGame->players[i].id != tokaidoGame->myId) {
            // If other players are sooner or equal us so this case will fail
            if (tokaidoGame->players[i].currentSite <= me->currentSite) {
                return false;
            }
        }
    }

    if (!path->sites[me->currentSite + 1].isFull) {
        *move = me->currentSite + 1;
        return true;
    }

    return false;
}

/**
 * If we had odd money and there is a SITE_MO between us and the next barrier
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign a new move to it
 * @return : if this case is not possible it will return false to continue with
 * the next case
 */
bool get_a_move_odd_money(TokaidoGame *tokaidoGame, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];

    if (me->money % 2 == 1) {
        if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame,
                SITE_MO, move)) {
            return true;
        }
    }
    return false;
}

/**
 * If we had most card and there is a SITE_RI between us and the next barrier
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign a new move to it
 * @return : if this case is not possible it will return false to continue with
 * the next case
 */
bool get_a_move_most_card(TokaidoGame *tokaidoGame, int *move) {
    bool haveMostCard = true;
    bool everyoneHasZeroCard = true;
    Player *me = &tokaidoGame->players[tokaidoGame->myId];

    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        if (tokaidoGame->players[i].id != tokaidoGame->myId) {
            if (count_card(&tokaidoGame->players[i]) >= count_card(me)) {
                haveMostCard = false;
            }
            if (count_card(&tokaidoGame->players[i]) != 0) {
                everyoneHasZeroCard = false;
            }
        }
    }

    if (haveMostCard || everyoneHasZeroCard) {
        if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame,
                SITE_RI, move)) {
            return true;
        }
    }

    return false;
}

/**
 * If there is a SITE_V2 between us and the next barrier
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign a new move to it
 * @return : if this case is not possible it will return false to continue with
 * the next case
 */
bool get_a_move_v2_between(TokaidoGame *tokaidoGame, int *move) {
    if (get_a_specific_site_between_us_and_next_barrier(tokaidoGame, SITE_V2,
            move)) {
        return true;
    }

    return false;
}

/**
 * Move forward to the earliest site which has room
 * @param tokaidoGame : the current game is running
 * @param move : the variable that we will assign a new move to it
 * @returnm : if this case is not possible it will return false
 */
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

/**
 * Get a specific site between us and the next barrier
 *
 * @param tokaidoGame : the current game is running
 * @param type : the site type we would like to find
 * @param move : assign the result we want to this variable
 * @return : if this case is not possible it will return false
 */
bool get_a_specific_site_between_us_and_next_barrier(TokaidoGame *tokaidoGame,
        SiteType type, int *move) {
    Player *me = &tokaidoGame->players[tokaidoGame->myId];
    Path *path = tokaidoGame->path;

    for (int i = me->currentSite + 1; i < path->siteCount; ++i) {
        if (path->sites[i].type == SITE_BARRIER) {
            break;
        }
        if (path->sites[i].type == type && !path->sites[i].isFull) {
            *move = i;
            return true;
        }
    }
    return false;
}

/**
 * Process the message in case it announces a player make a move
 * @param message : detail of the move message
 * @param tokaidoGame : the current game is running
 */
void player_make_a_move(String *message, TokaidoGame *tokaidoGame) {
    char *playerIdStringFormat = strtok(message->buffer + 3, ",");
    char *siteStringFormat = strtok(NULL, ",");
    char *pointStringFormat = strtok(NULL, ",");
    char *moneyStringFormat = strtok(NULL, ",");
    char *cardStringFormat = strtok(NULL, ",");

    if (playerIdStringFormat == NULL || siteStringFormat == NULL ||
            pointStringFormat == NULL || moneyStringFormat == NULL ||
            cardStringFormat == NULL) {
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

void update_status(int playerId, int siteIndex, int point, int money,
        int cardType, TokaidoGame *tokaidoGame) {
    Player *player = &tokaidoGame->players[playerId];
    Site *site = &tokaidoGame->path->sites[siteIndex];
    remove_player(player, &tokaidoGame->path->sites[player->currentSite]);
    add_player(player, site);
    player->point += point;
    player->money += money;
    if (site->type == SITE_V1) {
        player->v1++;
    } else if (site->type == SITE_V2) {
        player->v2++;
    }
    if (cardType != 0) {
        player->cards[cardType - 1]++;
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
    tokaidoGame->playerCount = argc - 3;

    FILE *deckFile = fopen(argv[1], "r");
    if (deckFile == NULL || ferror(deckFile)) {
        throw_error(DEALER_DECK);
    } else {
        String *deck = read_deck(deckFile);
        validate_deck(deck, tokaidoGame, DEALER_DECK);
        fclose(deckFile);
    }

    FILE *pathFile = fopen(argv[2], "r");
    if (pathFile == NULL || ferror(pathFile)) {
        throw_error(DEALER_PATH);
    } else {
        String *path = read_path(pathFile);
        validate_path(path, tokaidoGame, DEALER_PATH);
        fclose(pathFile);
    }
}

String *read_deck(FILE *stream) {
    String *deck = initialize_string();
    read_from_stream(deck, stream, DEALER_DECK);
    return deck;
}

void validate_deck(String *deck, TokaidoGame *tokaidoGame, Error deckError) {
    int cardCount = 0;
    char *cardsInDeck = "";
    cardCount = (int) strtol(deck->buffer, &cardsInDeck, 10);

    if (cardCount != strlen(cardsInDeck) ||
            strcmp(deck->buffer, "") == 0 ||
            cardCount < 4) {
        throw_error(deckError);
    }

    tokaidoGame->deck = malloc(sizeof(char) * cardCount + 1);
    for (int i = 0; i < cardCount; ++i) {
        if (cardsInDeck[i] < 'A' || cardsInDeck[i] > 'E') {
            throw_error(deckError);
        } else {
            tokaidoGame->deck[i] = cardsInDeck[i];
        }
    }
    tokaidoGame->deck[cardCount] = '\0';

    free_string(deck);
}

void start_player_process(char **argv, TokaidoGame *tokaidoGame) {
    tokaidoGame->players = initialize_players(tokaidoGame->playerCount);

    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        Player *player = &tokaidoGame->players[i];
        char playerCount[11];
        char playerId[11];
        sprintf(playerCount, "%d", tokaidoGame->playerCount);
        sprintf(playerId, "%d", i);
        if (pipe(player->readPipe) == -1) {
            throw_error(DEALER_STARTING_PROCESS);
        }
        if (pipe(player->writePipe) == -1) {
            throw_error(DEALER_STARTING_PROCESS);
        }
        player->pid = fork();
        if (player->pid == 0) {
            dup2(player->readPipe[READ], 0);
            close(player->readPipe[WRITE]);
            dup2(player->writePipe[WRITE], 1);
            close(player->writePipe[READ]);
            FILE *devNull = fopen("/dev/null", "w");
            if (devNull == NULL || ferror(devNull)) {
                throw_error(DEALER_STARTING_PROCESS);
            } else {
                dup2(fileno(devNull), 2);
            }
            if (execl(argv[3 + i], argv[3 + i], playerCount, playerId,
                    NULL) == -1) {
                throw_error(DEALER_STARTING_PROCESS);
            }
        } else {
            if (player->pid == -1) {
                throw_error(DEALER_STARTING_PROCESS);
            }
            close(player->readPipe[READ]);
            close(player->writePipe[WRITE]);
            player->inputStream = fdopen(player->readPipe[WRITE], "w");
            player->outputStream = fdopen(player->writePipe[READ], "r");
        }
    }
}

void start_dealer_game(TokaidoGame *tokaidoGame) {
    bool endGame = false;
    bool earlyEndGame = false;
    add_all_player_first_barrier(tokaidoGame);
    send_path_to_all_player(tokaidoGame);
    render(tokaidoGame, stdout);
    while (!endGame) {
        Player *nextTurnPlayer = get_next_turn_player(tokaidoGame);
        request_a_move(nextTurnPlayer);
        String *message = read_message(nextTurnPlayer->outputStream,
                DEALER_COMMUNICATIONS);
        dealer_processor(message, tokaidoGame, nextTurnPlayer, &earlyEndGame);
        if (earlyEndGame) {
            notice_early_game_over_to_all_players(tokaidoGame);
            throw_error(DEALER_COMMUNICATIONS);
        }
        render_player(nextTurnPlayer, stdout);
        render(tokaidoGame, stdout);
        endGame = tokaidoGame->path
                ->sites[tokaidoGame->path->siteCount - 1].isFull;
        free_string(message);
    }
    notice_end_game_to_all_players(tokaidoGame);
    render_final_score(tokaidoGame, stdout);
}

Player *get_next_turn_player(TokaidoGame *tokaidoGame) {
    Site *sites = tokaidoGame->path->sites;
    int playerId = -1;
    for (int i = 0; i < tokaidoGame->path->siteCount; ++i) {
        if (sites[i].visitingOffset != -1) {
            playerId = sites[i].visitingPlayersId[sites[i].visitingOffset];
            break;
        }
    }

    if (playerId == -1) {
        throw_error(DEALER_COMMUNICATIONS);
    }
    return &tokaidoGame->players[playerId];
}

void send_path_to_all_player(TokaidoGame *tokaidoGame) {
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        Player *player = &tokaidoGame->players[i];
        String *message = read_message(player->outputStream,
                DEALER_STARTING_PROCESS);
        if (strcmp(message->buffer, "^") == 0) {
            write_string_to_stream(tokaidoGame->path->stringFormat->buffer,
                    player->inputStream);
            write_char_to_stream('\n', player->inputStream);
        } else {
            throw_error(DEALER_STARTING_PROCESS);
        }
    }
}

void request_a_move(Player *player) {
    write_string_to_stream("YT\n", player->inputStream);
}

void dealer_processor(String *message, TokaidoGame *tokaidoGame,
        Player *nextTurnPlayer, bool *earlyEndGame) {
    Path *path = tokaidoGame->path;
    if (message->length > 2 && message->buffer[0] == 'D' &&
            message->buffer[1] == 'O') {
        char *error = "";
        int move = (int) strtol(message->buffer + 2, &error, 10);
        if (strcmp(error, "") != 0 || strcmp(message->buffer + 2, "") == 0) {
            *earlyEndGame = true;
        }
        if (is_move_valid(move, tokaidoGame, nextTurnPlayer)) {
            remove_player(nextTurnPlayer,
                    &path->sites[nextTurnPlayer->currentSite]);
            add_player(nextTurnPlayer, &path->sites[move]);
            int additionalPoints = 0;
            int changeInMoney = 0;
            int cardType = 0;
            switch (path->sites[move].type) {
                case SITE_MO:
                    nextTurnPlayer->money += 3;
                    changeInMoney += 3;
                    break;
                case SITE_V1:
                    nextTurnPlayer->v1++;
                    break;
                case SITE_V2:
                    nextTurnPlayer->v2++;
                    break;
                case SITE_DO:
                    additionalPoints = nextTurnPlayer->money / 2;
                    nextTurnPlayer->point += additionalPoints;
                    changeInMoney -= nextTurnPlayer->money;
                    nextTurnPlayer->money = 0;
                    break;
                case SITE_RI:
                    cardType = draw_card_from_deck(tokaidoGame,
                            nextTurnPlayer);
                    break;
                case SITE_BARRIER:
                    break;
            }
            notice_to_all_players(nextTurnPlayer->id, move, additionalPoints,
                    changeInMoney, cardType, tokaidoGame);
        } else {
            *earlyEndGame = true;
        }
    } else {
        *earlyEndGame = true;
    }
}

bool is_move_valid(int move, TokaidoGame *tokaidoGame,
        Player *nextTurnPlayer) {
    Path *path = tokaidoGame->path;
    if (move > nextTurnPlayer->currentSite &&
            move < path->siteCount &&
            !path->sites[move].isFull) {
        for (int i = nextTurnPlayer->currentSite + 1; i < move; ++i) {
            if (path->sites[i].type == SITE_BARRIER) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void notice_to_all_players(int playerId, int newSite, int additionalPoints,
        int changeInMoney, int cardType, TokaidoGame *tokaidoGame) {
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        FILE *playerInputStream = tokaidoGame->players[i].inputStream;
        write_string_to_stream("HAP", playerInputStream);
        write_int_to_stream(playerId, playerInputStream);
        write_char_to_stream(',', playerInputStream);
        write_int_to_stream(newSite, playerInputStream);
        write_char_to_stream(',', playerInputStream);
        write_int_to_stream(additionalPoints, playerInputStream);
        write_char_to_stream(',', playerInputStream);
        write_int_to_stream(changeInMoney, playerInputStream);
        write_char_to_stream(',', playerInputStream);
        write_int_to_stream(cardType, playerInputStream);
        write_char_to_stream('\n', playerInputStream);
    }
}

int draw_card_from_deck(TokaidoGame *tokaidoGame, Player *player) {
    char card = tokaidoGame->deck[tokaidoGame->deckOffset++];
    int cardType = card - 'A' + 1;
    player->cards[cardType - 1]++;
    if (tokaidoGame->deckOffset == strlen(tokaidoGame->deck)) {
        tokaidoGame->deckOffset = 0;
    }
    return cardType;
}

void notice_end_game_to_all_players(TokaidoGame *tokaidoGame) {
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        FILE *playerInputStream = tokaidoGame->players[i].inputStream;
        write_string_to_stream("DONE\n", playerInputStream);
    }
}

void notice_early_game_over_to_all_players(TokaidoGame *tokaidoGame) {
    for (int i = 0; i < tokaidoGame->playerCount; ++i) {
        FILE *playerInputStream = tokaidoGame->players[i].inputStream;
        write_string_to_stream("EARLY\n", playerInputStream);
    }
}
