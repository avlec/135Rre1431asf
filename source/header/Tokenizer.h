#ifndef TOKENIZER_H
#define TOKENIZER_H

#define DEFAULT_TOKEN_LENGTH 2048

char ** stot_d(char * string, int string_length, char * delimiters);
char ** stot(char * string, int string_length, char delimiter);
char ** cstot_d(const char * string, int string_length, char * delimiters);
char ** cstot(const char * string, int string_length, char delimiter);

#endif
