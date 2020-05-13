//
// Created by Jay Huynh on 4/28/20.
//

#include "path.h"

/**
 * Allocate memory for Path datatype
 * @return Path - the allocated memory
 */
Path *initialize_path() {
    Path *newPath = malloc(sizeof(Path) * 1);
    newPath->siteCount = 0;
    newPath->stringFormat = NULL;
    return newPath;
}

/**
 * Deallocate the Path memory that we allocated before.
 * We also call freesites to deallocated its site list
 * @param path : Path pointer datatype - the target path that we will
 * deallocate it
 */
void free_path(Path *path) {
    free_sites(path->sites);
    free(path);
}

/**
 * Allocate the amount of site for Path base on the input siteCount.
 *
 * @param siteCount : integer - allocate the amount of site base on this
 * @return : Site pointer datatype - this list of site
 */
Site *initialize_sites(int siteCount) {
    Site *newSites = malloc(sizeof(Site) * siteCount);
    for (int i = 0; i < siteCount; ++i) {
        newSites[i].index = i;
        newSites[i].isFull = false;
        newSites[i].visitingOffset = -1;
    }
    return newSites;
}

/**
 * Deallocate a list of site
 * @param sites : Site pointer datatype - the target list of site we want to
 * free
 */
void free_sites(Site *sites) {
    free(sites);
}

/**
 * Add a specific player into a site. The function will loop through all space
 * in visitingPlayersId array of the site. If there is a free place then assign
 * that player id into that space. It also set the currentSite of the user to
 * this site's index. Update the visitingOffset and set the isFull status if
 * needed
 *
 * @param player : Player pointer - the target player
 * @param site : Site pointer - the target site
 */
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

/**
 * Remove a player from a site by clear his id form the visitingPlayersId array
 * Update player's currentSite, the visitingOffset and isFull status also.
 *
 * @param player : Player pointer - the target player want to remove
 * @param site : Site pointer - the target site
 */
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
