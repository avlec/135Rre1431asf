#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "header/Tokenizer.h"

// Inaccessible behind the scenes functions.
char ** stot_f(char ** tokens, int string_length, char * delimiters);
bool 	stot_f_dc(char character, char * delimiters);

char ** stot_d(char * string, int string_length, char * delimiters) {
	char ** tokens = (char **) malloc(sizeof(char*)*DEFAULT_TOKEN_LENGTH);
	if(tokens == NULL)
		return NULL;
	// Give string.
	tokens[0] = string;

	return stot_f(tokens, string_length, delimiters);
}

char ** stot(char * string, int string_length, char delimiter) {
	char ** tokens = (char **) malloc(sizeof(char*)*DEFAULT_TOKEN_LENGTH);
	if(tokens == NULL)
		return NULL;
	// Give string.
	tokens[0] = string;

	char delimiters[2] = { delimiter, '\0' };

	return stot_f(tokens, string_length, delimiters);
}
char ** cstot_d(const char * string, int string_length, char * delimiters) {
	// Allocate room for our array of pointers to our given string
	char ** tokens = (char **) malloc(sizeof(char*)*DEFAULT_TOKEN_LENGTH);
	if(tokens == NULL)
		return NULL;
	// copy string into tokens[0]
	tokens[0] = (char *) malloc(sizeof(char)*string_length);
	memcpy(tokens[0], string, string_length);

	return stot_f(tokens, string_length, delimiters);
}

// sudo-inplace tokenizing
char ** cstot(const char * string, int string_length, char delimiter) {
	// Allocate room for our array of pointers to our given string
	char ** tokens = (char **) malloc(sizeof(char*)*DEFAULT_TOKEN_LENGTH);
	if(tokens == NULL)
		return NULL;
	// copy string into tokens[0]
	tokens[0] = (char *) malloc(sizeof(char)*string_length);
	memcpy(tokens[0], string, string_length);

	char delimiters[2] = { delimiter, '\0' };
	/* RIPPED */

	return stot_f(tokens, string_length, delimiters);
}

bool stot_d_c(char character, char * delimiters) {
	while((*delimiters) != '\0')
		if(character == *(delimiters++)) {
			return true;
		}
	return false;
}

char ** stot_f(char ** tokens, int string_length, char * delimiters) {
	// walk through the string, replace delimiter with '\0'
	// after replacing delimiter keep replacing till non delim character
	// add pointer to list of strings.
	bool last_was_delim = false;
	int token_index = 0;
	for(int i = 0; (i < string_length) || (tokens[0][i] == '\0'); ++i) {
		if(stot_d_c(tokens[0][i], delimiters)) { // replacing delim character
			if(!last_was_delim) { // Avoids unnecessary memory operations
				tokens[0][i] = '\0';
				last_was_delim = true;
			}
		} else {
			if(last_was_delim) {
				// assign current position to tokens[++token_index]
				tokens[++token_index] = &(tokens[0][i]);
				last_was_delim = false;
			}
		}
	}

	return tokens;
}
