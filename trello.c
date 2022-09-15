/* 
 * File:   trello.c
 * Author: joshmc@uab.edu
 *
 * Created on August 30, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trello.h"
#include "string_functions.h"

/* ================== Board Functions ================== */

int import_boards(board_type boards[], int *index) {
    char const filename[] = "Boards.csv";
    FILE *input;
    const char delim = ',';
    char line[line_length_max];
    char chunks[chunk_count_max][chunk_length_max];
    int card_count;
    
    input = fopen(filename, "r");
    if(input == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    } else {
        while(file_read_line(input, line) > 0) {
            if(string_split(line, delim, chunks) >= 3) {
                boards[*index].b_id = atoi(chunks[0]);
                strcpy(boards[*index].b_desc, chunks[1]);
                card_count = string_split(line, delim, chunks) - 2;
                for(int i = 0; i < card_count; i++) {
                    boards[*index].b_cards[i] = atoi(chunks[i + 2]);
                }
                boards[*index].b_cards[card_count] = 0;
                
                *index = *index + 1;
            } else {
                printf("Warning: invalid line (%s) in the input file\n", line);
            }
        }
    }
    
    fclose(input);
    
    return EXIT_SUCCESS;
}

int export_boards(char const filename[], board_type boards[], int index) {
    FILE *output;
    int locations[1];
    
    if(filename == NULL || string_find(filename, ".csv", locations) != 1) {
        printf("Error: invalid filename provided\n");
        return EXIT_FAILURE;
    } else if(boards == NULL) {
        printf("Error: invalid boards array provided\n");
        return EXIT_FAILURE;
    } else {
        output = fopen(filename, "w");
        if(output == NULL) {
            printf("Error: unable to open boards file\n");
            return EXIT_FAILURE;
        } else {
            for(int i = 0; i < index; i++) {
                fprintf(output, "%i,%s", boards[i].b_id, boards[i].b_desc);
                int x = 0;
                while(boards[i].b_cards[x] != 0) {
                    fprintf(output, ",%i", boards[i].b_cards[x]);
                    x++;
                }
                fprintf(output, "\n");
            }
        }
        
        fclose(output);        
    }
    
    return EXIT_SUCCESS;
}

int add_board(board_type boards[], int id, char desc[], int index) {
    char const filename[] = "Boards.csv";
    FILE *output;
    
    if (boards == NULL) {
        printf("Error: invalid boards array provided\n");
        return EXIT_FAILURE;
    } else if (index >= board_count_max) {
        printf("Error: Maximum amount of boards created\n");
        return EXIT_FAILURE;
    }
    
    if(id == 0) {
        printf("Generating new b_id\n");
        int newId = 1;
        int flag = 0;
        int idAllowed = 0;
        while(idAllowed == 0) {
            flag = 0;
            for(int i = 0; i < index; i++) {
                if(newId == boards[i].b_id) {
                    flag = 1;
                }
            }
            if(flag == 0) {
                idAllowed = 1;
            } else {
                newId++;
            }
        }
        id = newId;
        printf("New ID: %i\n", id);
    }
    
    output = fopen(filename, "a");
    if(output == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    }

    fprintf(output, "%i,%s\n", id, desc);

    fclose(output);
    
    return EXIT_SUCCESS;
}

int update_board(board_type boards[], int id, char desc[], int index) {
    int i = 0;
    while(id != boards[i].b_id) {
        i++;
    }
    strcpy(boards[i].b_desc, desc);
    export_boards("Boards.csv", boards, index);
    return EXIT_SUCCESS;
}

int archive_board(board_type boards[], int id, int *index) {
    int i = 0;
    while(id != boards[i].b_id) {
        i++;
    }
    
    FILE *archive;
    archive = fopen("Boards_Archive.csv", "a");
    if(archive == NULL) {
        printf("Error opening archive file\n");
        return EXIT_FAILURE;
    } else {
        fprintf(archive, "%i,%s", boards[i].b_id, boards[i].b_desc);
        int x = 0;
        while(boards[i].b_cards[x] != 0) {
            fprintf(archive, ",%i", boards[i].b_cards[x]);
            x++;
        }
        fprintf(archive, "\n");
        fclose(archive);
    }
    
    for(int fix = i; fix < (*index - 1); fix++) {
        boards[fix] = boards[fix + 1];
    }
    
    *index = *index - 1;
    
    export_boards("Boards.csv", boards, *index);
    
    return EXIT_SUCCESS;
}

void display_board(board_type boards[], int id) {
    int index = 0;
    int c = 0;
    while(id != boards[index].b_id) {
        index++;
        if(index > board_count_max) {
            printf("Error: invalid id provided\n");
            return;
        }
    }
    printf("\n");
    printf("Board ID:       %d\n", boards[index].b_id);
    printf("Board Desc:     %s\n", boards[index].b_desc);
    while(boards[index].b_cards[c] != 0) {
        printf("Card[%i]: %i\n", (c + 1), boards[index].b_cards[c]);
        c++;
    }
}

int add_card_to_board(board_type boards[], int index, int b_id, int c_id) {
    int i = 0;
    while(b_id != boards[i].b_id) {
        i++;
    }
    
    int c = 0;
    while(boards[i].b_cards[c] != 0) {
        c++;
    }
    
    boards[i].b_cards[c] = c_id;
    boards[i].b_cards[c + 1] = 0; 
    
    export_boards("Boards.csv", boards, index);
    
    return EXIT_SUCCESS;
}

int remove_card_from_board(board_type boards[], int index, int b_id, int c_id) {
    int i = 0;
    while(b_id != boards[i].b_id) {
        i++;
    }
    
    int c = 0;
    while(boards[i].b_cards[c] != c_id) {
        c++;
    }
    
    int last_card = 0;
    while(boards[i].b_cards[last_card] != 0) {
        last_card++;
    }
    
    for(int fix = c; fix < last_card; fix++) {
        boards[i].b_cards[fix] = boards[i].b_cards[fix + 1];
    }
    
    export_boards("Boards.csv", boards, index);
    
    return EXIT_SUCCESS;
}

/* ================== Card Functions ================== */

int import_cards(card_type cards[], int *index) {
    char const filename[] = "Cards.csv";
    FILE *input;
    const char delim = ',';
    char line[line_length_max];
    char chunks[chunk_count_max][chunk_length_max];
    int user_count;
    
    input = fopen(filename, "r");
    if(input == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    } else {
        while(file_read_line(input, line) > 0) {
            if(string_split(line, delim, chunks) >= 3) {
                cards[*index].c_id = atoi(chunks[0]);
                strcpy(cards[*index].c_desc, chunks[1]);
                switch(atoi(chunks[2])) {
                    case 0: 
                        cards[*index].c_state = backlog; 
                        break;
                    case 1: 
                        cards[*index].c_state = design;
                        break;
                    case 2:
                        cards[*index].c_state = build;
                        break;
                    case 3:
                        cards[*index].c_state = test;
                        break;
                    case 4:
                        cards[*index].c_state = done;
                        break;
                    case 5:
                        cards[*index].c_state = abandon;
                        break;
                    default:
                        cards[*index].c_state = unknown;
                        break;
                }
                user_count = string_split(line, delim, chunks) - 3;
                for(int i = 0; i < user_count; i++) {
                    cards[*index].c_users[i] = atoi(chunks[i + 3]);
                }
                cards[*index].c_users[user_count] = 0;
                
                *index = *index + 1;
            } else {
                printf("Warning: invalid line (%s) in the input file\n", line);
            }
        }
    }
    
    fclose(input);
    
    return EXIT_SUCCESS;
}

int export_cards(char const filename[], card_type cards[], int index) {
    FILE *output;
    int locations[1];
    
    if(filename == NULL || string_find(filename, ".csv", locations) != 1) {
        printf("Error: invalid filename provided\n");
        return EXIT_FAILURE;
    } else if (cards == NULL) {
        printf("Error: invalid cards array provided\n");
        return EXIT_FAILURE;
    } else {
        output = fopen(filename, "w");
        if(output == NULL) {
            printf("Error: unable to open cards file\n");
            return EXIT_FAILURE;
        } else {
            for(int i = 0; i < index; i++) {
                fprintf(output, "%i,%s,%i", cards[i].c_id, cards[i].c_desc, (int)cards[i].c_state);
                int x = 0;
                while(cards[i].c_users[x] != 0) {
                    fprintf(output, ",%i", cards[i].c_users[x]);
                    x++;
                }
                fprintf(output, "\n");
            }
        }
        
        fclose(output);
    }
    
    return EXIT_SUCCESS;
}

int add_card(card_type cards[], int index, int board, int id, char desc[], int user, card_states state) {
    char const filename[] = "Cards.csv";
    FILE *output;
    
    if(cards == NULL) {
        printf("Error: invalid cards array provided\n");
        return EXIT_FAILURE;
    } else if (index > card_count_max) {
        printf("Error: Maximum amount of cards created\n");
        return EXIT_FAILURE;
    } else if(board == 0) {
        printf("Error: invalid board ID provided\n");
        return EXIT_FAILURE;
    }
    
    if(id == 0) {
        printf("Generate new c_id\n");
        int newId = 1;
        int flag = 0;
        int idAllowed = 0;
        while(idAllowed == 0) {
            flag = 0;
            for(int i = 0; i < index; i++) {
                if(newId == cards[i].c_id) {
                    flag = 1;
                }
            }
            if(flag == 0) {
                idAllowed = 1;
            } else {
                newId++;
            }
        }
        id = newId;
        printf("New c_id: %i\n", id);
    }
    
    if(user == 0) {
        user = 1;
    }
    
    output = fopen(filename, "a");
    if(output == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    }
    
    fprintf(output, "%i,%s,%d,%i\n", id, desc, (int)state, user);
    
    fclose(output);
    
    return id;
}

int update_card(card_type cards[], int index, int id, char desc[], int user, card_states state) {
    int i = 0;
    while(id != cards[i].c_id) {
        i++;
    }
    
    if(strcmpi(desc, " ") != 0) {
        strcpy(cards[i].c_desc, desc);
    }
    
    if(user != 0) {
        cards[i].c_users[0] = user;
    }
    
    if(state != 0) {
        switch((int)state) {
            case 0:
                cards[i].c_state = backlog;
                break;
            case 1:
                cards[i].c_state = design;
                break;
            case 2:
                cards[i].c_state = build;
                break;
            case 3:
                cards[i].c_state = test;
                break;
            case 4:
                cards[i].c_state = done;
                break;
            case 5:
                cards[i].c_state = abandon;
                break;
            default:
                cards[i].c_state = unknown;
                break;
        }
    }
    
    export_cards("Cards.csv", cards, index);
    
    return EXIT_SUCCESS;
}

int archive_card(card_type cards[], int id, int *index) {
    int i = 0;
    while(id != cards[i].c_id) {
        i++;
    }
    
    FILE *archive;
    archive = fopen("Cards_Archive.csv", "a");
    if(archive == NULL) {
        printf("Error openeing cards archive file\n");
        return EXIT_FAILURE;
    } else {
        fprintf(archive, "%i,%s,%d", cards[i].c_id, cards[i].c_desc, cards[i].c_state);
        int x = 0;
        while(cards[i].c_users[i] != 0) {
            fprintf(archive, ",%i", cards[i].c_users[x]);
            x++;
        }
        fprintf(archive, "\n");
        fclose(archive);
    }
    
    for(int fix = i; fix < (*index - 1); fix++) {
        cards[fix] = cards[fix + 1];
    }
    
    *index = *index - 1;
    
    export_cards("Cards.csv", cards, *index);
    
    return EXIT_SUCCESS;
}

void display_card(card_type cards[], int id) {
    int i = 0;
    while(id != cards[i].c_id) {
        i++;
        if(i > card_count_max) {
            printf("Error: invalid id provided\n");
            return;
        }
    }
    
    printf("\n");
    printf("Card ID:        %d\n", cards[i].c_id);
    printf("Card Desc:      %s\n", cards[i].c_desc);
    char state[8];
    switch(cards[i].c_state) {
        case backlog: strcpy(state, "backlog"); break;
        case design: strcpy(state, "design"); break;
        case build: strcpy(state, "build"); break;
        case test: strcpy(state, "test"); break;
        case done: strcpy(state, "done"); break;
        case abandon: strcpy(state, "abandon"); break;
        case unknown: strcpy(state, "unknown"); break;
    }
    printf("Card State:     %s\n", state);
    int u = 0;
    while(cards[i].c_users[u] != 0) {
        printf("User[%i]: %i\n", (u + 1), cards[i].c_users[u]);
        u++;
    }
}

int add_user_to_card(card_type cards[], int index, int c_id, int u_id) {
    int i = 0;
    while(c_id != cards[i].c_id) {
        i++;
    }
    
    int u = 0;
    while(cards[i].c_users[u] != 0) {
        u++;
    }
    
    cards[i].c_users[u] = u_id;
    cards[i].c_users[u + 1] = 0;
    
    export_cards("Cards.csv", cards, index);
    
    return EXIT_SUCCESS;
}

int remove_user_from_card(card_type cards[], int index, int c_id, int u_id) {
    int i = 0;
    while(c_id != cards[i].c_id) {
        i++;
    }
    
    int u = 0;
    while(cards[i].c_users[u] != u_id) {
        u++;
    }
    
    int last_user = 0;
    while(cards[i].c_users[last_user] != 0) {
        last_user++;
    }
    
    for(int fix = u; fix < last_user; fix++) {
        cards[i].c_users[fix] = cards[i].c_users[fix + 1];
    }
    
    export_cards("Cards.csv", cards, index);
    
    return EXIT_SUCCESS;
}

/* ================== User Functions ================== */

int import_users(user_type users[], int *index) {
    char const filename[] = "Users.csv";
    FILE *input;
    const char delim = ',';
    char line[line_length_max];
    char chunks[chunk_count_max][chunk_length_max];
    
    input = fopen(filename, "r");
    if(input == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    } else {
        while(file_read_line(input, line) > 0) {
            if(string_split(line, delim, chunks) >= 3) {
                users[*index].u_id = atoi(chunks[0]);
                strcpy(users[*index].username, chunks[1]);
                users[*index].u_level = atoi(chunks[2]);
                
                *index = *index + 1;
            } else {
                printf("Warning: invalid line (%s) in the input file\n", line);
            }
        }
    }
    
    fclose(input);
    
    return EXIT_SUCCESS;
}

int export_users(char const filename[], user_type users[], int index) {
    FILE *output;
    int locations[1];
    
    if(filename == NULL || string_find(filename, ".csv", locations) != 1) {
        printf("Error: invalid filename provided\n");
        return EXIT_FAILURE;
    } else if(users == NULL) {
        printf("Error: invalid users array provided\n");
        return EXIT_FAILURE;
    } else {
        output = fopen(filename, "w");
        if(output == NULL) {
            printf("Error: unable to open users file\n");
            return EXIT_FAILURE;
        } else {
            for(int i = 0; i < index; i++) {
                fprintf(output, "%i,%s,%i\n", users[i].u_id, users[i].username, users[i].u_level);
            }
        }
        
        fclose(output);
    }
    
    return EXIT_SUCCESS;
}

int add_user(user_type users[], int id, char name[], int index) {
    char const filename[] = "Users.csv";
    FILE *output;
    
    if(users == NULL) {
        printf("Error: invalid users array provided\n");
        return EXIT_FAILURE;
    } else if (index >= user_count_max) {
        printf("Error: Maximum amount of users created\n");
        return EXIT_FAILURE;
    }
    
    if(id == 0) {
        printf("Generating new u_id\n");
        int newId = 1;
        int flag;
        int idAllowed = 0;
        while(idAllowed == 0) {
            flag = 0;
            for(int i = 0; i < index; i++) {
                if(newId == users[i].u_id) {
                    flag = 1;
                }
            }
            if(flag == 0) {
                idAllowed = 1;
            } else {
                newId++;
            }
        }
        id = newId;
        printf("New ID: %i\n", id);
    }
    
    output = fopen(filename, "a");
    if(output == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    }
    
    fprintf(output, "%i,%s,%i\n", id, name, 0);
    
    fclose(output);
    
    return id;
}

int update_user(user_type users[], int id, char name[], int index) {
    int i = 0;
    while(id != users[i].u_id) {
        i++;
        if(i > user_count_max) {
            printf("Error: user not found!\n");
            return EXIT_FAILURE;
        }
    }
    
    strcpy(users[i].username, name);
    
    export_users("Users.csv", users, index);
    
    return EXIT_SUCCESS;
}

int archive_user(user_type users[], int id, int *index) {
    int i = 0;
    while(id != users[i].u_id) {
        i++;
    }
    
    FILE *archive;
    archive = fopen("Users_Archive.csv", "a");
    if(archive == NULL) {
        printf("Error opening users archive file\n");
        return EXIT_FAILURE;
    } else {
        fprintf(archive, "%i,%s,%i\n", users[i].u_id, users[i].username, users[i].u_level);
    }
    
    for(int fix = i; fix < (*index - 1); fix++) {
        users[fix] = users[fix + 1];
    }
    
    *index = *index - 1;
    
    export_users("Users.csv", users, *index);
    
    return EXIT_SUCCESS;
}