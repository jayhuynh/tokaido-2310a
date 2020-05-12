//
// Created by Jay Huynh on 4/28/20.
//

#include "helper.h"

String *initialize_string() {
    String *newString = malloc(sizeof(String) * 1);
    newString->buffer = malloc(sizeof(char) * 40);
    newString->capacity = 40;
    newString->length = 0;
    return newString;
}

void write_string_to_stream(char *input, FILE *stream) {
    fprintf(stream, "%s", input);
    fflush(stream);
}

void write_char_to_stream(char input, FILE *stream) {
    fprintf(stream, "%c", input);
    fflush(stream);
}

void write_int_to_stream(int input, FILE *stream) {
    fprintf(stream, "%d", input);
    fflush(stream);
}

void read_from_stream(String *output, FILE *stream, Error type) {
    size_t current = 0;
    (output->buffer)[0] = '\0';
    int input;
    while (input = fgetc(stream), input != EOF && input != '\n') {
        if (current > output->capacity - 2) {
            size_t newCap = (size_t) (output->capacity * 1.5);
            void *newBlock = realloc(output->buffer, newCap);
            output->capacity = newCap;
            output->buffer = newBlock;
        }
        (output->buffer)[current] = (char) input;
        output->length++;
        (output->buffer)[++current] = '\0';
        if (type == DEALER_STARTING_PROCESS && (char) input == '^') {
            break;
        }
    }
    if (input == EOF && output->length == 0) {
        throw_error(type);
    }
}

void free_string(String *string) {
    free(string->buffer);
    free(string);
}

int string_to_int(char *input, Error type) {
    char *error = "";
    int result = (int) strtol(input, &error, 10);
    if (strcmp(error, "") != 0 || strcmp(input, "") == 0) {
        throw_error(type);
    }
    return result;
}

int catch_signal(int signal, void (*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    return sigaction(signal, &action, NULL);
}

void handler_sighup(int signal) {
    kill(-1 * getpid(), SIGKILL);
    exit(6);
}
