//
// Created by Jay Huynh on 4/28/20.
//

#include "helper.h"

/**
 * Allocate a String datatype and also allocate a initial buffer which is
 * a character array for that String datatype
 * @return String
 */
String *initialize_string() {
    String *newString = malloc(sizeof(String) * 1);
    newString->buffer = malloc(sizeof(char) * 40);
    newString->capacity = 40;
    newString->length = 0;
    return newString;
}

/**
 * Write a string into a specific stream which is a file pointer
 * @param input: pointer to char - the string would like to print
 * @param stream: file pointer - the target stream would like to print into
 */
void write_string_to_stream(char *input, FILE *stream) {
    fprintf(stream, "%s", input);
    fflush(stream);
}

/**
 * Write a character into a specific stream which is a file pointer
 * @param input: char - the character would like to print
 * @param stream: file pointer - the target stream would like to print into
 */
void write_char_to_stream(char input, FILE *stream) {
    fprintf(stream, "%c", input);
    fflush(stream);
}

/**
 * Write an integer number into a specific stream which is a file pointer
 * @param input: integer - the integer number would like to print
 * @param stream: file pointer - the target stream would like to print into
 */
void write_int_to_stream(int input, FILE *stream) {
    fprintf(stream, "%d", input);
    fflush(stream);
}

/**
 * Read the data from a specific stream and write it into
 * an output String datatype.
 *
 * The function will throw the corresponding error base on the input error type
 * in parameter
 *
 * @param output : String datatype - function will write result into this
 * @param stream : File pointer - the target stream that function will read
 * @param type : Error enum type - function will throw error base on this
 */
void read_from_stream(String *output, FILE *stream, Error type) {
    size_t current = 0;
    (output->buffer)[0] = '\0';
    int input;
    while (input = fgetc(stream), input != EOF && input != '\n') {
        /**
         * Function also resize the String if its capacity not enough for
         * the current message
         */
        if (current > output->capacity - 2) {
            size_t newCap = (size_t) (output->capacity * 1.5);
            void *newBlock = realloc(output->buffer, newCap);
            output->capacity = newCap;
            output->buffer = newBlock;
        }
        (output->buffer)[current] = (char) input;
        output->length++;
        (output->buffer)[++current] = '\0';
        /**
         * Break in case we meet special character "^"
         * I check the error type have to = DEALER_STARTING_PROCESS in order to
         * make sure that only the dealer will meet this case. Because the
         * error DEALER_STARTING_PROCESS only used for dealer process
         */
        if (type == DEALER_STARTING_PROCESS && (char) input == '^') {
            break;
        }
    }
    /**
     * Break in case of end of file and throw the corresponding error
     */
    if (input == EOF && output->length == 0) {
        throw_error(type);
    }
}

/**
 * Deallocate a specific String that we allocated before. We also have to
 * deallocate its buffer
 * @param string : String pointer datatype - the target String to deallocate
 */
void free_string(String *string) {
    free(string->buffer);
    free(string);
}

/**
 * Convert a string/character array and return corresponding integer result of
 * that string by using strtol() function. It also checks for error and throw
 * error in case the error not empty or we have empty input string
 *
 * @param input : a string - input string to convert
 * @param type : Error enum type - throwing error while conversion base on this
 * @return : an integer value - the value of the input string in case there are
 * no error happen
 */
int string_to_int(char *input, Error type) {
    char *error = "";
    int result = (int) strtol(input, &error, 10);
    if (strcmp(error, "") != 0 || strcmp(input, "") == 0) {
        throw_error(type);
    }
    return result;
}

/**
 * Custom catch signal function. It will regist a specific signal as well as
 * its handler function
 *
 * @param signal : integer - the signal we want to catch
 * @param handler : function pointer - the handler function we want to execute
 * when we catch the signal
 * @return : integer - the status when we regist with sigaction function. If
 * this value == -1 -> fail to regist to catch this signal
 */
int catch_signal(int signal, void (*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    return sigaction(signal, &action, NULL);
}

/**
 * Handler function for SIGHUP signal. It will kill all the child processes
 * base on the group id. After kill all child processes, it will exit with
 * status code 6. I use 6 because the spec said won't care about the exit
 * status in this case
 *
 * @param signal : integer - the signal we got, but we won't use it in this
 * situation
 */
void handler_sighup(int signal) {
    kill(-1 * getpid(), SIGKILL);
    exit(6);
}
