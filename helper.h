//
// Created by Jay Huynh on 4/28/20.
//

#ifndef TOKAIDO_2310A_HELPER_H
#define TOKAIDO_2310A_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "unistd.h"
#include "signal.h"
#include "stdbool.h"
#define READ 0
#define WRITE 1

volatile bool isAChildDead;

typedef struct {
    char* buffer;
    size_t capacity;
    int length;
} String;

String *initialize_string();
void write_string_to_stream(char *input, FILE *stream);
void write_char_to_stream(char input, FILE *stream);
void write_int_to_stream(int input, FILE *stream);
void read_from_stream(String *output, FILE* stream, Error type);
void free_string(String *string);
int string_to_int(char *input, Error type);
int catch_signal(int signal, void (*handler)(int));
void handler_sigchild(int signal);

#endif //TOKAIDO_2310A_HELPER_H
