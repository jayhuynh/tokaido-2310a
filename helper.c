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

void write_to_stream(char *input, FILE *stream) {
    fprintf(stream, "%s", input);
    fflush(stream);
}

void read_from_stream(String *output, FILE *stream) {
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
    }
}