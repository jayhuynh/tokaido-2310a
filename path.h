//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_PATH_H
#define TOKAIDO_2310A_PATH_H

#include "player.h"
#include <stdlib.h>

/**
 * An enum type stand for all type of site
 */
typedef enum {
    SITE_MO = 0,
    SITE_V1 = 1,
    SITE_V2 = 2,
    SITE_DO = 3,
    SITE_RI = 4,
    SITE_BARRIER = 5
} SiteType;

/**
 * A Site datatype which contains
 */
typedef struct {
    SiteType type;  // Its site type
    char *label; // Its site type but in string format
    int capacity; // Its capacity
    int *visitingPlayersId; // The array of id of players who are currently
    // visiting this site
    int visitingOffset; // The offset in the visitingPlayersId array
    int index; // The index of this site in map
    bool isFull; // Check if the site is full or not
} Site;

/**
 * The Path for playing. The path will contain a list of sites
 */
typedef struct {
    int siteCount; // The quantity of sites in map
    Site *sites; // The list of sites
    String *stringFormat; // String format of this Path
} Path;

Path *initialize_path();

void free_path(Path *path);

Site *initialize_sites(int siteCount);

void free_sites(Site *sites);

void add_player(Player *player, Site *site);

void remove_player(Player *player, Site *site);

#endif //TOKAIDO_2310A_PATH_H
