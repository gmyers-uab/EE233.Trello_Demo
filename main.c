/* 
 * File:   main.c
 * Author: joshmc@uab.edu
 *
 * Created on August 30, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trello.h"
#include "string_functions.h"

/*
 *  
 */
int main(int argc, char** argv) {
    board_type boards[board_count_max];
    card_type cards[card_count_max];
    user_type users[user_count_max];
    
    int results = 0;
    int boards_index = 0;
    int cards_index = 0;
    int users_index = 0;
    
    import_boards(boards, &boards_index);
    import_cards(cards, &cards_index);
    import_users(users, &users_index);
    
    if(argc < 2) {
        help();
    } else if ((argc == 2) && (strcmpi(argv[1], "/h") == 0)) {
        help();
    } else if ((argc == 2) && (strcmpi(argv[1], "/?") == 0)) {
        help();
    } else if ((argc >= 4) && (strcmpi(argv[1], "/board") == 0)) {
        if((argc == 4) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Creating new board, auto b_id\n");
            results = add_board(boards, 0, argv[3], boards_index);
        } else if((argc == 5) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Creating new board\n");
            results = add_board(boards, atoi(argv[3]), argv[4], boards_index);
        } else if ((argc == 5) && (strcmpi(argv[2], "/addcard") == 0)) {
            printf("Adding card to board\n");
            results = add_card_to_board(boards, boards_index, atoi(argv[3]), atoi(argv[4]));
        } else if ((argc == 5) && (strcmpi(argv[2], "/removecard") == 0)) {
            printf("Removing card from board\n");
            results = remove_card_from_board(boards, boards_index, atoi(argv[3]), atoi(argv[4]));
        } else if ((argc == 6) && (strcmpi(argv[2], "/movecard") == 0)) {
            printf("Moving card from b_id_1 to b_id_2");
            results = add_card_to_board(boards, boards_index, atoi(argv[4]), atoi(argv[5]));
            results = remove_card_from_board(boards, boards_index, atoi(argv[3]), atoi(argv[5]));
        } else if ((argc == 5) && (strcmpi(argv[2], "/update") == 0)) {
            printf("Update existing board description\n");
            results = update_board(boards, atoi(argv[3]), argv[4], boards_index);
        } else if ((argc == 4) && (strcmpi(argv[2], "/archive") == 0)) {
            printf("Archive board\n");
            results = archive_board(boards, atoi(argv[3]), &boards_index);
        } else if ((argc == 4) && (strcmpi(argv[2], "/display") == 0)) {
            printf("Displaying board\n");
            display_board(boards, atoi(argv[3]));
        } else {
            printf("Incorrect arguments. Displaying help.\n");
            help();
        }
    } else if ((argc >= 5) && (strcmpi(argv[1], "/card") == 0)) {
        if(strcmpi(argv[2], "/add") == 0) {
            if(argc == 5) {
                printf("Add new card, auto c_id, auto u_id\n");
                int newID = add_card(cards, cards_index, atoi(argv[3]), 0, argv[4], 0, backlog);
                results = add_card_to_board(boards, boards_index, atoi(argv[3]), newID);
            } else if (argc == 6) {
                printf("Add new card, auto c_id\n");
                int newID = add_card(cards, cards_index, atoi(argv[3]), 0, argv[5], atoi(argv[4]), backlog);
                results = add_card_to_board(boards, boards_index, atoi(argv[3]), newID);
            } else if (argc == 7) {
                printf("Add new card\n");
                int newID = add_card(cards, cards_index, atoi(argv[3]), atoi(argv[5]), argv[6], atoi(argv[4]), backlog);
                results = add_card_to_board(boards, boards_index, atoi(argv[3]), newID);
            } else {
                printf("Incorrect arguments. Displaying help.\n");
                help();
            }
        } else if ((argc == 6) && (strcmpi(argv[2], "/adduser") == 0)) {
            printf("Adding user to card\n");
            results = add_user_to_card(cards, cards_index, atoi(argv[4]), atoi(argv[5]));
        } else if ((argc == 6) && (strcmpi(argv[2], "/removeuser") == 0)) {
            printf("Removing user from card\n");
            results = remove_user_from_card(cards, cards_index, atoi(argv[4]), atoi(argv[5]));
        } else if ((argc == 7) && (strcmpi(argv[2], "/moveuser") == 0)) {
            printf("Moving user from c_id_1 to c_id_2\n");
            results = add_user_to_card(cards, cards_index, atoi(argv[5]), atoi(argv[6]));
            results = remove_user_from_card(cards, cards_index, atoi(argv[4]), atoi(argv[6]));
        } else if ((argc == 7) && (strcmpi(argv[2], "/update") == 0)) {
            if(strcmpi(argv[3], "/desc") == 0) {
                printf("Update description of card\n");
                results = update_card(cards, cards_index, atoi(argv[5]), argv[6], 0, 0);
            } else if (strcmpi(argv[3], "/state") == 0) {
                printf("Update state of the card\n");
                results = update_card(cards, cards_index, atoi(argv[5]), " ", 0, atoi(argv[6]));
            } else if (strcmpi(argv[3], "/user") == 0) {
                printf("Update user of the card\n");
                results = update_card(cards, cards_index, atoi(argv[5]), " ", atoi(argv[6]), 0);
            } else {
                printf("Incorrect arguments. Displaying help.\n");
                help();
            }
        } else if ((argc == 5) && (strcmpi(argv[2], "/archive") == 0)) {
            printf("Archive card\n");
            results = archive_card(cards, atoi(argv[4]), &cards_index);
        } else if ((argc == 5) && (strcmpi(argv[2], "/display") == 0)) {
            printf("Display card\n");
            display_card(cards, atoi(argv[4]));
        } else {
            printf("Incorrect arguments. Displaying help.\n");
            help();
        }
    } else if ((argc >= 4) && (strcmpi(argv[1], "/user") == 0)) {
        if((argc == 4) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Add new user, auto u_id\n");
            results = add_user(users, 0, argv[3], users_index);
        } else if ((argc == 5) && (strcmpi(argv[2], "/add") == 0)) {
            printf("Add new user\n");
            results = add_user(users, atoi(argv[3]), argv[4], users_index);
        } else if ((argc == 5) && (strcmpi(argv[2], "/update") == 0)) {
            printf("Update user info\n");
            results = update_user(users, atoi(argv[3]), argv[4], users_index);
        } else if ((argc == 5) && (strcmpi(argv[2], "/archive") == 0)) {
            printf("Archive user\n");
            results = archive_user(users, atoi(argv[3]), &users_index);
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

void help(void) {
    printf("\nUsage:\n");
    printf("    /board /add b_desc\n");
    printf("    /board /add b_id b_desc\n");
    printf("    /board /addcard b_id c_id\n");
    printf("    /board /removecard b_id c_id\n");
    printf("    /board /movecard b_id_1 b_id_2 c_id\n");
    printf("    /board /update b_id b_desc\n");
    printf("    /board /archive b_id\n");
    printf("    /board /display b_id\n");
    printf("    /card /add b_id c_desc\n");
    printf("    /card /add b_id u_id c_desc\n");
    printf("    /card /add b_id u_id c_id c_desc\n");
    printf("    /card /adduser b_id c_id u_id\n");
    printf("    /card /removeuser b_id c_id u_id\n");
    printf("    /card /moveuser b_id c_id_1 c_id_2 u_id\n");
    printf("    /card /update /desc b_id c_id c_desc\n");
    printf("    /card /update /state b_id c_id state\n");
    printf("    /card /update /user b_id c_id u_id\n");
    printf("    /card /archive b_id c_id\n");
    printf("    /card /display b_id c_id\n");
    printf("    /user /add username\n");
    printf("    /user /add u_id username\n");
    printf("    /user /update u_id username\n");
    printf("    /user /archive u_id username\n");
    printf("    /h\n");
    printf("    /?\n");
}



