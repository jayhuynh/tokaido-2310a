//
// Created by Jay Huynh on 4/28/20.
//

#include "path.h"

Path *initialize_path() {
    Path *newPath = malloc(sizeof(Path) * 1);
    newPath->siteCount = 0;
    return newPath;
}

Site *initialize_sites(int siteCount) {
    Site *newSites = malloc(sizeof(Site) * siteCount);
    return newSites;
}

void add_player(Player *player, Site *site) {
    for (int i = 0; i < site->capacity; ++i) {
        if (site->visitingPlayersId[i] == -1) {
            site->visitingPlayersId[i] = player->id;
            break;
        }
    }
}

void remove_player(Player *player, Site *site) {
    for (int i = 0; i < site->capacity; ++i) {
        if (site->visitingPlayersId[i] == player->id) {
            site->visitingPlayersId[i] = -1;
            break;
        }
    }
}