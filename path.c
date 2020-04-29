//
// Created by Jay Huynh on 4/28/20.
//

#include "path.h"

Path *initialize_path() {
    Path *newPath = malloc(sizeof(Path) * 1);
    newPath->siteCount = 0;
    return newPath;
}