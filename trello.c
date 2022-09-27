/* 
 * File: trello.c
 * Copy: Copyright (c) 2022 Josh McDowell
 * BlazerID: joshmc
 * Vers: 1.0.0 15/09/2022 JKM - Original Coding
 * Desc: Trello demo functionality
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trello.h"
#include "Core.h"

/* ================== List Functions ================== */
/* 
 * Name: int import_lists(list_type lists[], int *index)
 * Desc: Imports all the lists from file "Lists.csv" into lists array
 * Args: 
 *  list_type lists[] - array of struct list_type for imported lists to be added
 *  int *index - pointer to index of lists to be updated for every list imported
 *  return - failure or success
 */
int import_lists(list_type lists[], int *index) {
    char const filename[] = "Lists.csv";
    FILE *input;
    const char delim = ',';
    char line[line_length_max];
    char chunks[chunk_count_max][chunk_length_max];
    int card_count;
    int card_iter;
    
    input = fopen(filename, "r");
    if (input == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    } else {
        while (file_read_line(input, line) > 0) {
            if (string_split(line, delim, chunks) >= 3) {
                lists[*index].list_id = atoi(chunks[0]);
                strcpy(lists[*index].list_desc, chunks[1]);
                card_count = string_split(line, delim, chunks) - 2;
                for (card_iter = 0; card_iter < card_count; card_iter++) {
                    lists[*index].list_cards[card_iter] = atoi(chunks[card_iter + 2]);
                }
                lists[*index].list_cards[card_count] = 0;
                
                *index = *index + 1;
            } else {
                printf("Warning: invalid line (%s) in the input file\n", line);
            }
        }
    }
    
    fclose(input);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: int export_lists(char const filename[], list_type lists[], int index)
 * Desc: Exports all the lists in the input array to the specified filename
 * Args: 
 *  char const filename[] - filename to be exported to, must be a .csv file
 *  list_type lists[] - array of struct list_type to be exported
 *  int index - index of lists
 *  return - failure or success
 */
int export_lists(char const filename[], list_type lists[], int index) {
    FILE *output;
    int locations[1];
    int list_iter;
    
    if (filename == NULL || string_find(filename, ".csv", locations) != 1) {
        printf("Error: invalid filename provided\n");
        return EXIT_FAILURE;
    } else if (lists == NULL) {
        printf("Error: invalid lists array provided\n");
        return EXIT_FAILURE;
    } else {
        output = fopen(filename, "w");
        if (output == NULL) {
            printf("Error: unable to open lists file\n");
            return EXIT_FAILURE;
        } else {
            for (list_iter = 0; list_iter < index; list_iter++) {
                fprintf(output, "%i,%s", lists[list_iter].list_id, lists[list_iter].list_desc);
                int card_iter = 0;
                while (lists[list_iter].list_cards[card_iter] != 0) {
                    fprintf(output, ",%i", lists[list_iter].list_cards[card_iter]);
                    card_iter++;
                }
                fprintf(output, "\n");
            }
        }
        
        fclose(output);        
    }
    
    return EXIT_SUCCESS;
}

/* 
 * Name: int add_list(list_type lists[], int id, char desc[], int index)
 * Desc: Adds a new list with ID and decription to array of existing lists
 * Args: 
 *  list_type lists[] - array of struct list_type to be appended to
 *  int index - index of lists
 *  int id - new list ID, auto generated list ID will be created if 0 is passed
 *  char desc[] - list description
 *  return - failure or success
 */
int add_list(list_type lists[], int index, int id, char desc[]) {
    char const filename[] = "Lists.csv";
    FILE *output;
    int list_iter;
    
    if (lists == NULL) {
        printf("Error: invalid lists array provided\n");
        return EXIT_FAILURE;
    } else if (index >= list_count_max) {
        printf("Error: Maximum amount of lists created\n");
        return EXIT_FAILURE;
    }
    
    if (id == 0) {
        printf("Generating new list_id\n");
        int newId = 1;
        int flag = 0;
        int idAllowed = 0;
        while (idAllowed == 0) {
            flag = 0;
            for (list_iter = 0; list_iter < index; list_iter++) {
                if (newId == lists[list_iter].list_id) {
                    flag = 1;
                }
            }
            if (flag == 0) {
                idAllowed = 1;
            } else {
                newId++;
            }
        }
        id = newId;
        printf("New ID: %i\n", id);
    }
    
    output = fopen(filename, "a");
    if (output == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    }

    fprintf(output, "%i,%s\n", id, desc);

    fclose(output);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: update_list(list_type lists[], int id, char desc[], int index)
 * Desc: Updates the list description
 * Args:
 *  list_type lists[] - array of struct list_type
 *  int index - index of lists
 *  int id - ID of list to be updated
 *  char desc[] - new description of the list
 *  return - failure or success
 */
int update_list(list_type lists[], int index, int id, char desc[]) {
    int i = 0;
    while (id != lists[i].list_id) {
        i++;
    }
    strcpy(lists[i].list_desc, desc);
    export_lists("Boards.csv", lists, index);
    return EXIT_SUCCESS;
}

/* 
 * Name: archive_list(list_type lists[], int id, int *index)
 * Desc: Removes a list from Boards.csv and appends it to Boards_Archive.csv
 * Args:
 *  list_type lists[] - array of struct list_type
 *  int *index - pointer to index of lists to be updated after list has been archived
 *  int id - ID of list to be archived
 *  return - failure or success
 */
int archive_list(list_type lists[], int *index, int id) {
    int list_loc = 0;
    while (id != lists[list_loc].list_id) {
        list_loc++;
    }
    
    FILE *archive;
    archive = fopen("Lists_Archive.csv", "a");
    if (archive == NULL) {
        printf("Error opening archive file\n");
        return EXIT_FAILURE;
    } else {
        fprintf(archive, "%i,%s", lists[list_loc].list_id, lists[list_loc].list_desc);
        int card_iter = 0;
        while (lists[list_loc].list_cards[card_iter] != 0) {
            fprintf(archive, ",%i", lists[list_loc].list_cards[card_iter]);
            card_iter++;
        }
        fprintf(archive, "\n");
        fclose(archive);
    }
    
    int fix;
    for (fix = list_loc; fix < (*index - 1); fix++) {
        lists[fix] = lists[fix + 1];
    }
    
    *index = *index - 1;
    
    export_lists("Lists.csv", lists, *index);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: display_list(list_type lists[], int id)
 * Desc: Displays a list to the console
 * Args:
 *  list_type lists[] - array of struct list_type
 *  int id - ID of list to be displayed
 */
void display_list(list_type lists[], int id) {
    int list_loc = 0;
    int card_iter = 0;
    while (id != lists[list_loc].list_id) {
        list_loc++;
        if (list_loc > list_count_max) {
            printf("Error: invalid id provided\n");
            return;
        }
    }
    printf("\n");
    printf("List ID:       %d\n", lists[list_loc].list_id);
    printf("List Desc:     %s\n", lists[list_loc].list_desc);
    while (lists[list_loc].list_cards[card_iter] != 0) {
        printf("Card[%i]: %i\n", (card_iter + 1), lists[list_loc].list_cards[card_iter]);
        card_iter++;
    }
}

/* 
 * Name: add_card_to_list(list_type lists[], int index, int list_id, int card_id)
 * Desc: Adds a card ID to array of cards on a list
 * Args:
 *  list_type lists[] - array of struct list_type
 *  int index - index of lists
 *  int list_id - ID of list
 *  int card_id - ID of card
 *  return - failure or success
 */
int add_card_to_list(list_type lists[], int index, int list_id, int card_id) {
    int list_loc = 0;
    while (list_id != lists[list_loc].list_id) {
        list_loc++;
    }
    
    int card_count = 0;
    while (lists[list_loc].list_cards[card_count] != 0) {
        card_count++;
    }
    
    lists[list_loc].list_cards[card_count] = card_id;
    lists[list_loc].list_cards[card_count + 1] = 0; 
    
    export_lists("Lists.csv", lists, index);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: remove_card_from_list(list_type lists[], int index, int list_id, int card_id)
 * Desc: Removes a card ID from array of cards on a list
 * Args:
 *  list_type lists[] - array of struct list_type
 *  int index - index of lists
 *  int list_id - ID of list
 *  int card_id - ID of card
 *  return - failure or success
 */
int remove_card_from_list(list_type lists[], int index, int list_id, int card_id) {
    int list_loc = 0;
    while (list_id != lists[list_loc].list_id) {
        list_loc++;
    }
    
    int card_loc = 0;
    while (lists[list_loc].list_cards[card_loc] != card_id) {
        card_loc++;
    }
    
    int card_count = 0;
    while (lists[list_loc].list_cards[card_count] != 0) {
        card_count++;
    }
    
    int fix;
    for (fix = card_loc; fix < card_count; fix++) {
        lists[list_loc].list_cards[fix] = lists[list_loc].list_cards[fix + 1];
    }
    
    export_lists("Lists.csv", lists, index);
    
    return EXIT_SUCCESS;
}

/* ================== Card Functions ================== */
/* 
 * Name: int import_cards(card_type cards[], int *index)
 * Desc: Imports all the cards from file "Cards.csv" into cards array
 * Args: 
 *  card_type cards[] - array of struct card_type for imported cards to be added
 *  int *index - pointer to index of cards to be updated for every card imported
 *  return - failure or success
 */
int import_cards(card_type cards[], int *index) {
    char const filename[] = "Cards.csv";
    FILE *input;
    const char delim = ',';
    char line[line_length_max];
    char chunks[chunk_count_max][chunk_length_max];
    int user_count;
    int user_iter;
    
    input = fopen(filename, "r");
    if (input == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    } else {
        while (file_read_line(input, line) > 0) {
            if (string_split(line, delim, chunks) >= 3) {
                cards[*index].card_id = atoi(chunks[0]);
                strcpy(cards[*index].card_desc, chunks[1]);
                switch(atoi(chunks[2])) {
                    case 0: 
                        cards[*index].card_priority = low; 
                        break;
                    case 1: 
                        cards[*index].card_priority = medium;
                        break;
                    case 2:
                        cards[*index].card_priority = high;
                        break;
                    default:
                        cards[*index].card_priority = unknown;
                        break;
                }
                user_count = string_split(line, delim, chunks) - 3;
                for (user_iter = 0; user_iter < user_count; user_iter++) {
                    cards[*index].card_users[user_iter] = atoi(chunks[user_iter + 3]);
                }
                cards[*index].card_users[user_count] = 0;
                
                *index = *index + 1;
            } else {
                printf("Warning: invalid line (%s) in the input file\n", line);
            }
        }
    }
    
    fclose(input);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: int export_cards(char const filename[], card_type cards[], int index)
 * Desc: Exports all the cards in the input array to the specified filename
 * Args: 
 *  char const filename[] - filename to be exported to, must be a .csv file
 *  card_type cards[] - array of struct card_type to be exported
 *  int index - index of cards
 *  return - failure or success
 */
int export_cards(char const filename[], card_type cards[], int index) {
    FILE *output;
    int locations[1];
    int card_iter;
    int user_iter;
    
    if (filename == NULL || string_find(filename, ".csv", locations) != 1) {
        printf("Error: invalid filename provided\n");
        return EXIT_FAILURE;
    } else if (cards == NULL) {
        printf("Error: invalid cards array provided\n");
        return EXIT_FAILURE;
    } else {
        output = fopen(filename, "w");
        if (output == NULL) {
            printf("Error: unable to open cards file\n");
            return EXIT_FAILURE;
        } else {
            for (card_iter = 0; card_iter < index; card_iter++) {
                fprintf(output, "%i,%s,%i", cards[card_iter].card_id, cards[card_iter].card_desc, (int)cards[card_iter].card_priority);
                user_iter = 0;
                while (cards[card_iter].card_users[user_iter] != 0) {
                    fprintf(output, ",%i", cards[card_iter].card_users[user_iter]);
                    user_iter++;
                }
                fprintf(output, "\n");
            }
        }
        
        fclose(output);
    }
    
    return EXIT_SUCCESS;
}

/* 
 * Name: int add_card(card_type cards[], int index, int list, int id, char desc[], int user, card_states state)
 * Desc: Adds a new card with ID, description, state, and user to array of existing cards
 * Args: 
 *  card_type cards[] - array of struct card_type to be appended to
 *  int index - index of cards
 *  int list - list ID that new card will be added to
 *  int id - new card ID, auto generated card ID will be created if 0 is passed
 *  char desc[] - card description
 *  int user - user ID to be added to the card
 *  priority_level priority - priority of the new card
 *  return - ID of the card that was just added (might have been auto-generated)
 */
int add_card(card_type cards[], int index, int id, char desc[], int user, priority_level priority) {
    char const filename[] = "Cards.csv";
    FILE *output;
    int card_iter;
    
    if (cards == NULL) {
        printf("Error: invalid cards array provided\n");
        return EXIT_FAILURE;
    } else if (index > card_count_max) {
        printf("Error: Maximum amount of cards created\n");
        return EXIT_FAILURE;
    }
    
    if (id == 0) {
        printf("Generate new card_id\n");
        int newId = 1;
        int flag = 0;
        int idAllowed = 0;
        while (idAllowed == 0) {
            flag = 0;
            for (card_iter = 0; card_iter < index; card_iter++) {
                if (newId == cards[card_iter].card_id) {
                    flag = 1;
                }
            }
            if (flag == 0) {
                idAllowed = 1;
            } else {
                newId++;
            }
        }
        id = newId;
        printf("New card_id: %i\n", id);
    }
    
    if (user == 0) {
        user = 1;
    }
    
    output = fopen(filename, "a");
    if (output == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    }
    
    fprintf(output, "%i,%s,%d,%i\n", id, desc, (int)priority, user);
    
    fclose(output);
    
    return id;
}

/* 
 * Name: update_card(card_type cards[], int index, int id, char desc[], int user, card_states state)
 * Desc: Updates the card description
 * Args:
 *  card_type cards[] - array of struct card_type
 *  int index - index of cards
 *  int id - ID of card to be updated
 *  char desc[] - new description of the card
 *  int user - new user of the card
 *  priority_level priority - new priority of the card
 *  return - failure or success
 */
int update_card(card_type cards[], int index, int id, char desc[], priority_level priority) {
    int i = 0;
    while (id != cards[i].card_id) {
        i++;
    }
    
    if (strcmpi(desc, " ") != 0) {
        strcpy(cards[i].card_desc, desc);
    }
    
    if (priority != -1) {
        cards[i].card_priority = priority;
    }
    
    export_cards("Cards.csv", cards, index);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: int archive_card(card_type cards[], int id, int *index)
 * Desc: Removes a card from Cards.csv and appends it to Cards_Archive.csv
 * Args:
 *  card_type cards[] - array of struct card_type
 *  int *index - pointer to index of cards to be updated after card has been archived
 *  int id - ID of card to be archived
 *  return - failure or success
 */
int archive_card(card_type cards[], int *index, int id) {
    int card_loc = 0;
    while (id != cards[card_loc].card_id) {
        card_loc++;
    }
    
    FILE *archive;
    archive = fopen("Cards_Archive.csv", "a");
    if (archive == NULL) {
        printf("Error openeing cards archive file\n");
        return EXIT_FAILURE;
    } else {
        fprintf(archive, "%i,%s,%d", cards[card_loc].card_id, cards[card_loc].card_desc, cards[card_loc].card_priority);
        int user_iter = 0;
        while (cards[card_loc].card_users[user_iter] != 0) {
            fprintf(archive, ",%i", cards[card_loc].card_users[user_iter]);
            user_iter++;
        }
        fprintf(archive, "\n");
        fclose(archive);
    }
    
    int fix;
    for (fix = card_loc; fix < (*index - 1); fix++) {
        cards[fix] = cards[fix + 1];
    }
    
    *index = *index - 1;
    
    export_cards("Cards.csv", cards, *index);
    
    return EXIT_SUCCESS;
}

/* 
 * Name: display_card(card_type cards[], int id)
 * Desc: Displays a card to the console
 * Args:
 *  card_type cards[] - array of struct card_type
 *  int id - ID of card to be displayed
 */
void display_card(card_type cards[], int id) {
    int card_loc = 0;
    while (id != cards[card_loc].card_id) {
        card_loc++;
        if (card_loc > card_count_max) {
            printf("Error: invalid id provided\n");
            return;
        }
    }
    
    printf("\n");
    printf("Card ID:        %d\n", cards[card_loc].card_id);
    printf("Card Desc:      %s\n", cards[card_loc].card_desc);
    char priority[8];
    switch(cards[card_loc].card_priority) {
        case low: strcpy(priority, "low"); break;
        case medium: strcpy(priority, "medium"); break;
        case high: strcpy(priority, "high"); break;
        case unknown: strcpy(priority, "unknown"); break;
    }
    printf("Card State:     %s\n", priority);
    int user_iter = 0;
    while (cards[card_loc].card_users[user_iter] != 0) {
        printf("User[%i]: %i\n", (user_iter + 1), cards[card_loc].card_users[user_iter]);
        user_iter++;
    }
}

/*
 * Name: add_user_to_card(card_type cards[], int index, int card_id, int user_id)
 * Desc: Adds a user ID to array of users on a card
 * Args:
 *  card_type cards[] - array of struct card_type
 *  int index - index of cards
 *  int card_id - ID of card
 *  int user_id - ID of user
 *  return - failure or success
 */
int add_user_to_card(card_type cards[], int index, int card_id, int user_id) {
    int i = 0;
    while (card_id != cards[i].card_id) {
        i++;
    }
    
    int u = 0;
    while (cards[i].card_users[u] != 0) {
        u++;
    }
    
    cards[i].card_users[u] = user_id;
    cards[i].card_users[u + 1] = 0;
    
    export_cards("Cards.csv", cards, index);
    
    return EXIT_SUCCESS;
}

/*
 * Name: remove_user_from_card(card_type cards[], int index, int card_id, int user_id)
 * Desc: Removes a user ID from array of users on a card
 * Args:
 *  card_type cards[] - array of struct card_type
 *  int index - index of cards
 *  int card_id - ID of card
 *  int user_id - ID of user
 *  return - failure or success
 */
int remove_user_from_card(card_type cards[], int index, int card_id, int user_id) {
    int i = 0;
    while (card_id != cards[i].card_id) {
        i++;
    }
    
    int u = 0;
    while (cards[i].card_users[u] != user_id) {
        u++;
    }
    
    int last_user = 0;
    while (cards[i].card_users[last_user] != 0) {
        last_user++;
    }
    
    int fix;
    for (fix = u; fix < last_user; fix++) {
        cards[i].card_users[fix] = cards[i].card_users[fix + 1];
    }
    
    export_cards("Cards.csv", cards, index);
    
    return EXIT_SUCCESS;
}

/* ================== User Functions ================== */
/*
 * Name: int import_users(user_type users[], int *index)
 * Desc: Imports all the users from file "Users.csv" into users array
 * Args:
 *  user_type users[] - array of struct user_type for imported users to be added
 *  int *index - pointer to index of users to be updated for every user imported
 *  return - failure or success 
 */
int import_users(user_type users[], int *index) {
    char const filename[] = "Users.csv";
    FILE *input;
    const char delim = ',';
    char line[line_length_max];
    char chunks[chunk_count_max][chunk_length_max];
    
    input = fopen(filename, "r");
    if (input == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    } else {
        while (file_read_line(input, line) > 0) {
            if (string_split(line, delim, chunks) >= 3) {
                users[*index].user_id = atoi(chunks[0]);
                strcpy(users[*index].user_name, chunks[1]);
                users[*index].user_level = atoi(chunks[2]);
                
                *index = *index + 1;
            } else {
                printf("Warning: invalid line (%s) in the input file\n", line);
            }
        }
    }
    
    fclose(input);
    
    return EXIT_SUCCESS;
}

/*
 * Name: int export_users(char const filename[], user_type users[], int *index)
 * Desc: Exports all the users in the input array to the specified filename
 * Args:
 *  char const filename[] - filename to be exported to, must be a .csv file
 *  user_type users[] - array of struct user_type to be exported
 *  int index - index of users
 *  return - failure or success
 */
int export_users(char const filename[], user_type users[], int index) {
    FILE *output;
    int locations[1];
    int i;
    
    if (filename == NULL || string_find(filename, ".csv", locations) != 1) {
        printf("Error: invalid filename provided\n");
        return EXIT_FAILURE;
    } else if (users == NULL) {
        printf("Error: invalid users array provided\n");
        return EXIT_FAILURE;
    } else {
        output = fopen(filename, "w");
        if (output == NULL) {
            printf("Error: unable to open users file\n");
            return EXIT_FAILURE;
        } else {
            for (i = 0; i < index; i++) {
                fprintf(output, "%i,%s,%i\n", users[i].user_id, users[i].user_name, users[i].user_level);
            }
        }
        
        fclose(output);
    }
    
    return EXIT_SUCCESS;
}

/*
 * Name: int add_user(user_type users[], int id, char name[], int index)
 * Desc: Adds a new user with ID and name to array of existing users
 * Args:
 *  user_type users[] - array of struct user_type to be appended to
 *  int index - index of users
 *  int id - new user ID, auto generated user ID will be created if 0 is passed
 *  char name[] - user name
 *  return - failure or success
 */
int add_user(user_type users[], int index, int id, char name[]) {
    char const filename[] = "Users.csv";
    FILE *output;
    int i;
    
    if (users == NULL) {
        printf("Error: invalid users array provided\n");
        return EXIT_FAILURE;
    } else if (index >= user_count_max) {
        printf("Error: Maximum amount of users created\n");
        return EXIT_FAILURE;
    }
    
    if (id == 0) {
        printf("Generating new user_id\n");
        int newId = 1;
        int flag;
        int idAllowed = 0;
        while (idAllowed == 0) {
            flag = 0;
            for (i = 0; i < index; i++) {
                if (newId == users[i].user_id) {
                    flag = 1;
                }
            }
            if (flag == 0) {
                idAllowed = 1;
            } else {
                newId++;
            }
        }
        id = newId;
        printf("New ID: %i\n", id);
    }
    
    output = fopen(filename, "a");
    if (output == NULL) {
        printf("Error: invalid filename\n");
        return EXIT_FAILURE;
    }
    
    fprintf(output, "%i,%s,%i\n", id, name, 0);
    
    fclose(output);
    
    return id;
}

/*
 * Name: int update_user(user_type users[], int id, char name[], int index)
 * Desc: Updates the user name
 * Args:
 *  user_type users[] - array of struct user_type
 *  int index - index of cards
 *  int id - ID of user to be updated
 *  char name[] - new name of user
 *  return - failure or success
 */
int update_user(user_type users[], int index, int id, char name[]) {
    int i = 0;
    while (id != users[i].user_id) {
        i++;
        if (i > user_count_max) {
            printf("Error: user not found!\n");
            return EXIT_FAILURE;
        }
    }
    
    strcpy(users[i].user_name, name);
    
    export_users("Users.csv", users, index);
    
    return EXIT_SUCCESS;
}

/*
 * Name: int archive_user(user_type users[], int id, int *index)
 * Desc: Removes a user from Users.csv and appends it to Users_Archive.csv
 * Args:
 *  user_type users[] - array of struct user_type
 *  int id - ID of user to be archived
 *  int *index - pointer to index of users to be updated after card has been archived
 *  return - failure or success
 */
int archive_user(user_type users[], int *index, int id) {
    int i = 0;
    while (id != users[i].user_id) {
        i++;
    }
    
    FILE *archive;
    archive = fopen("Users_Archive.csv", "a");
    if (archive == NULL) {
        printf("Error opening users archive file\n");
        return EXIT_FAILURE;
    } else {
        fprintf(archive, "%i,%s,%i\n", users[i].user_id, users[i].user_name, users[i].user_level);
    }
    
    int fix;
    for (fix = i; fix < (*index - 1); fix++) {
        users[fix] = users[fix + 1];
    }
    
    *index = *index - 1;
    
    export_users("Users.csv", users, *index);
    
    return EXIT_SUCCESS;
}