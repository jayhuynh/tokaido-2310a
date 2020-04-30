//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_HELPER_H
#define TOKAIDO_2310A_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include "path.h"

typedef struct {
    char* buffer;
    size_t capacity;
    int length;
} String;

void debug_site(Site *site);

String *initialize_string();
void write_string_to_stream(char *input, FILE *stream);
void write_char_to_stream(char input, FILE *stream);
void read_from_stream(String *output, FILE* stream);

#endif //TOKAIDO_2310A_HELPER_H
