/* 
 * File:   string_functions.c
 * Author: joshmc@uab.edu
 *
 * Created on August 30, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trello.h"
#include "Core.h"

int file_read_line(FILE *input, char line[]) {
    char chunk;
    int count = 0;
    
    if(input != NULL) {
        while((fscanf(input, "%c", &chunk) != EOF) && (chunk != '\n') && (count < chunk_length_max - 1)) {
            line[count] = chunk;
            count++;
        }
    } else {
        printf("Error: invalid file pointer\n");
    }
    
    line[count] = '\0';
    
    return(count);
}

int string_split(char contents[], char delimiter, char chunks[][chunk_length_max]) {
    int contents_index;
    int chunks_index;  // this is the "word" or string index
    int chunk_index;   // this is the character index of a particular word
    
    contents_index = 0;
    chunks_index = 0;
    chunk_index = 0;
    
    while((contents[contents_index] != '\0') && (chunks_index < chunk_count_max)) {
        if(contents[contents_index] == delimiter) {
            chunks[chunks_index][chunk_index] = '\0';
            chunks_index++;
            chunk_index = 0;
        } else {
            if(chunks_index < (chunk_length_max - 1)) {
                chunks[chunks_index][chunk_index] = contents[contents_index];
                chunk_index++;
            } else {
                
            }
            
        }
        contents_index++;
    }
    chunks[chunks_index][chunk_index] = '\0';
    
    return(chunks_index + 1);
}

int string_find(char const contents[], char search[], int locations[]) {
    int contents_length;
    int search_length;
    int contents_index;
    int search_index;
    int match;
    int count;
    
    contents_length = string_length(contents);
    search_length = string_length(search);
    contents_index = 0;
    search_index = 0;
    match = 1;
    count = 0;
    
    if(contents_length < 0) {
        printf("Error: no valid contents string\n");
    } else if(search_length < 0) {
        printf("Error: no valid search string\n");
    } else if(contents_length < search_length) {
        printf("Error: search string longer than contents string\n");
    } else {
        while(contents_index < (contents_length - search_length + 1)) {
            match = 1;
            search_index = 0;
            while((match == 1) && (search_index < search_length)) {
                if(contents[contents_index + search_index] == search[search_index]) {
                    search_index++;
                } else {
                    match = 0;
                }
            }
            if(match == 1) {
                locations[count] = contents_index;
                count++;
                contents_index = contents_index + search_index;
            } else {
                contents_index++;
            }
        }
    }
    
    return(count);
}

int string_length(const char source[]) {
    int results;
    results = 0;
    
    while(source[results] != '\0') {
        results++;
    }
    
    return(results);
}