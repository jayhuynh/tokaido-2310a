//
// Created by Jay Huynh on 4/28/20.
//

#include "helper.h"

bool is_number(char* input){
    for (int i = 0; i < strlen(input); ++i) {
        if(!isdigit(input[i])){
            return false;
        }
    }
    return true;
}