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
    for (int i = 0; i < siteCount; ++i) {
        newSites[i].index = i;
        newSites[i].isFull = false;
    }
    return newSites;
}

void add_player(Player *player, Site *site) {
    for (int i = 0; i < site->capacity; ++i) {
        if (site->visitingPlayersId[i] == -1) {
            site->visitingPlayersId[i] = player->id;
            player->currentSite = site->index;
            if (site->type == V1) {
                player->v1++;
            } else if (site->type == V2) {
                player->v2++;
            }
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
            player->currentSite = -1;
            site->isFull = false;
            break;
        }
    }
}