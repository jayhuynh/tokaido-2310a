//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_PATH_H
#define TOKAIDO_2310A_PATH_H

#include "player.h"
#include <stdlib.h>

typedef enum {
    Mo = 0,
    V1 = 1,
    V2 = 2,
    Do = 3,
    Ri = 4,
    Barrier = 5
} SiteType;

typedef struct {
    SiteType type;
    int capacity;
    Player *visitingPlayers;
} Site;

typedef struct {
    int siteCount;
    Site *sites;
} Path;

Path *initialize_path();
Site *initialize_sites(int siteCount);

#endif //TOKAIDO_2310A_PATH_H
