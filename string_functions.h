/* 
 * File:   string_functions.h
 * Author: joshmc@uab.edu
 *
 * Created on August 30, 2022
 */

#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H

#define line_length_max 100
#define chunk_count_max 20
#define chunk_length_max 500

int file_read_line(FILE *input, char line[]);
int string_split(char contents[], char delimiter, char chunks[][chunk_length_max]);
int string_find(char const contents[], char search[], int locations[]);
int string_length(const char source[]);

#endif /* STRING_FUNCTIONS_H */

