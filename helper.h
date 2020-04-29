//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_HELPER_H
#define TOKAIDO_2310A_HELPER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* buffer;
    size_t capacity;
    int length;
} String;

String *initialize_string();
void write_to_stream(char *input, FILE *stream);
void read_from_stream(String *output, FILE* stream);

#endif //TOKAIDO_2310A_HELPER_H
