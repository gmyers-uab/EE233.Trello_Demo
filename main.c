/* 
 * File: main.c
 * Copy: Copyright (c) 2022 Josh McDowell
 * BlazerID: joshmc
 * Vers: 1.0.0 15/09/2022 JKM - Original Coding
 * Desc: Driver for EE233: Trello Demo
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trello.h"
#include "Core.h"

/* 
 * Name: int main(int argc, char** argv)
 * Desc: Used to drive functions found in the trello demo. Functions are called
 *  from the command line and passed through if/else block to call the correct
 *  function and arguments.
 */
int main(int argc, char** argv) {
    list_type lists[list_count_max];
    card_type cards[card_count_max];
    user_type users[user_count_max];
    
    int results = 0;
    int lists_index = 0;
    int cards_index = 0;
    int users_index = 0;
    int index;
    
    import_lists(lists, &lists_index);
    import_cards(cards, &cards_index);
    import_users(users, &users_index);
    
    printf("lists_index = %i\n", lists_index);
    printf("cards_index = %i\n", cards_index);
    printf("users_index = %i\n", users_index);
    
    printf("Argument count: %d\n", argc );
    for (index = 0; index < argc; index++) {
        printf("argv[ %d ]: %s\n", index, argv[ index ]);
    }
    printf("\n");
    
    if(argc < 2) {
        help();
    } else if ((argc == 2) && (strcmpi(argv[1], "/h") == 0)) {
        help();
    } else if ((argc == 2) && (strcmpi(argv[1], "/?") == 0)) {
        help();
    } else if ((argc >= 4) && (strcmpi(argv[1], "/list") == 0)) {
        if((argc == 4) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Creating new list, auto list_id\n");
            results = add_list(lists, lists_index, 0, argv[3]);
        } else if((argc == 5) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Creating new list\n");
            results = add_list(lists, lists_index, atoi(argv[3]), argv[4]);
        } else if ((argc == 5) && (strcmpi(argv[2], "/update") == 0)) {
            printf("Update existing list description\n");
            results = update_list(lists, lists_index, atoi(argv[3]), argv[4]);
        } else if ((argc == 5) && (strcmpi(argv[2], "/addcard") == 0)) {
            printf("Adding card to list, auto card_id\n");
            int newID = add_card(cards, cards_index, 0, argv[4], 0, low);
            results = add_card_to_list(lists, lists_index, atoi(argv[3]), newID);
        } else if((argc == 6) && (strcmpi(argv[2], "/addcard") == 0)) {
            printf("Adding card to list\n");
            results = add_card(cards, cards_index, atoi(argv[4]), argv[5], 0, low);
            results = add_card_to_list(lists, lists_index, atoi(argv[3]), atoi(argv[4]));
        } else if ((argc == 5) && (strcmpi(argv[2], "/removecard") == 0)) {
            printf("Removing card from list\n");
            results = remove_card_from_list(lists, lists_index, atoi(argv[3]), atoi(argv[4]));
        } else if ((argc == 6) && (strcmpi(argv[2], "/movecard") == 0)) {
            printf("Moving card from list_id_1 to list_id_2");
            results = add_card_to_list(lists, lists_index, atoi(argv[4]), atoi(argv[5]));
            results = remove_card_from_list(lists, lists_index, atoi(argv[3]), atoi(argv[5]));
        } else if ((argc == 4) && (strcmpi(argv[2], "/archive") == 0)) {
            printf("Archive list\n");
            results = archive_list(lists, &lists_index, atoi(argv[3]));
        } else if ((argc == 4) && (strcmpi(argv[2], "/display") == 0)) {
            printf("Displaying list\n");
            display_list(lists, cards, users, atoi(argv[3]));
        } else {
            printf("Incorrect arguments. Displaying help.\n");
            help();
        }
    } else if ((argc >= 4) && (strcmpi(argv[1], "/card") == 0)) {
        if((argc == 6) && (strcmpi(argv[2], "/update") == 0)) {
            if (strcmpi(argv[3], "/desc") == 0) {
                printf("Update description of the card\n");
                results = update_card(cards, cards_index, atoi(argv[4]), argv[5], -1);
            } else if (strcmpi(argv[3], "/priority") == 0) {
                printf("Update priority of the card\n");
                priority_level new_priority = -1;
                if (strcmpi(argv[5], "low") == 0) {
                    new_priority = low;
                } else if (strcmpi(argv[5], "medium") == 0) {
                    new_priority = medium;
                } else if (strcmpi(argv[5], "high") == 0) {
                    new_priority = high;
                } else {
                    printf("Invalid priority level.\n");
                }
                results = update_card(cards, cards_index, atoi(argv[4]), " ", new_priority);
            } else {
                printf("Incorrect arguments. Displaying help.\n");
                help();
            }
        } else if ((argc == 5) && (strcmpi(argv[2], "/adduser") == 0)) {
            printf("Adding user to card\n");
            results = add_user_to_card(cards, cards_index, atoi(argv[3]), atoi(argv[4]));
        } else if ((argc == 5) && (strcmpi(argv[2], "/removeuser") == 0)) {
            printf("Removing user from card\n");
            results = remove_user_from_card(cards, cards_index, atoi(argv[3]), atoi(argv[4]));
        } else if ((argc == 6) && (strcmpi(argv[2], "/moveuser") == 0)) {
            printf("Moving user from card_id_1 to card_id_2\n");
            results = add_user_to_card(cards, cards_index, atoi(argv[4]), atoi(argv[5]));
            results = remove_user_from_card(cards, cards_index, atoi(argv[3]), atoi(argv[5]));
        } else if ((argc == 4) && (strcmpi(argv[2], "/archive") == 0)) {
            printf("Archive card\n");
            results = archive_card(cards, &cards_index, atoi(argv[3]));
        } else if ((argc == 4) && (strcmpi(argv[2], "/display") == 0)) {
            printf("Display card\n");
            display_card(cards, users, atoi(argv[3]));
        } else {
            printf("Incorrect arguments. Displaying help.\n");
            help();
        }
    } else if ((argc >= 4) && (strcmpi(argv[1], "/user") == 0)) {
        if((argc == 4) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Add new user, auto user_id\n");
            results = add_user(users, users_index, 0, argv[3]);
        } else if ((argc == 5) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Add new user\n");
            results = add_user(users, users_index, atoi(argv[3]), argv[4]);
        } else if ((argc == 5) && (strcmpi(argv[2], "/update") == 0)) {
            printf("Update user info\n");
            results = update_user(users, users_index, atoi(argv[3]), argv[4]);
        } else if ((argc == 4) && (strcmpi(argv[2], "/archive") == 0)) {
            printf("Archive user\n");
            results = archive_user(users, &users_index, atoi(argv[3]));
        } else {
            printf("Incorrect arguments. Displaying help.\n");
            help();
        }
    } else {
        printf("Incorrect arguments. Displaying help.\n");
        help();
    }
    
    return (EXIT_SUCCESS);
}


/* 
 * Name: void main(void)
 * Desc: Used to provide a help screen to the user. Displays all possible
 *  arguments that can be passed to the program from the command line. 
 */
void help(void) {
    printf("\nUsage:\n");
    printf("    /list /add list_desc\n");
    printf("    /list /add list_id list_desc\n");
    printf("    /list /update list_id list_desc\n");
    printf("    /list /addcard list_id card_desc\n");
    printf("    /list /addcard list_id card_id card_desc\n");
    printf("    /list /removecard list_id card_id\n");
    printf("    /list /movecard list_id_1 list_id_2 card_id\n");
    printf("    /list /archive list_id\n");
    printf("    /list /display list_id\n");
    
    printf("    /card /update /desc card_id card_desc\n");
    printf("    /card /update /priority card_id priority_level\n");
    printf("    /card /adduser card_id user_id\n");
    printf("    /card /removeuser card_id user_id\n");
    printf("    /card /moveuser card_id_1 card_id_2 user_id\n");
    printf("    /card /archive card_id\n");
    printf("    /card /display card_id\n");
    
    printf("    /user /add user_name\n");
    printf("    /user /add user_id user_name\n");
    printf("    /user /update user_id user_name\n");
    printf("    /user /archive user_id\n");
    
    printf("    /h\n");
    printf("    /?\n");
}



