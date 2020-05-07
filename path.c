//
// Created by Jay Huynh on 4/28/20.
//

#include "path.h"

Path *initialize_path() {
    Path *newPath = malloc(sizeof(Path) * 1);
    newPath->siteCount = 0;
    newPath->stringFormat = NULL;
    return newPath;
}

void free_path(Path *path) {
    free_sites(path->sites);
    free(path);
}

Site *initialize_sites(int siteCount) {
    Site *newSites = malloc(sizeof(Site) * siteCount);
    for (int i = 0; i < siteCount; ++i) {
        newSites[i].index = i;
        newSites[i].isFull = false;
        newSites[i].visitingOffset = -1;
    }
    return newSites;
}

void free_sites(Site *sites) {
    free(sites);
}

void add_player(Player *player, Site *site) {
    for (int i = 0; i < site->capacity; ++i) {
        if (site->visitingPlayersId[i] == -1) {
            site->visitingPlayersId[i] = player->id;
            site->visitingOffset++;
            player->currentSite = site->index;
            if (i == site->capacity - 1) {
                site->isFull = true;
            }
            break;
        }
    }
}

void remove_player(Player *player, Site *site) {
    for (int i = 0; i < site->capacity; ++i) {
        if (site->visitingPlayersId[i] == player->id) {
            site->visitingPlayersId[i] = -1;
            site->visitingOffset--;
            player->currentSite = -1;
            site->isFull = false;
            break;
        }
    }
}
