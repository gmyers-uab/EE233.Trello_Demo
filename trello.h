/* 
 * File:   trello.h
 * Author: joshmc@uab.edu
 *
 * Created on August 30, 2022
 */

#ifndef TRELLO_H
#define TRELLO_H

#define board_count_max 5
#define card_count_max 100
#define user_count_max 10

typedef enum {
    backlog, design, build, test, done, abandon, unknown
} card_states;

typedef struct {
    int b_id;
    char b_desc[50];
    int b_cards[20];
} board_type;

typedef struct {
    int c_id;
    char c_desc[50];
    card_states c_state;
    int c_users[4];
} card_type;

typedef struct {
    int u_id;
    char username[20];
    int u_level;
} user_type;

void help(void);

int import_boards(board_type boards[], int *index);
int export_boards(char const filename[], board_type boards[], int index);
int add_board(board_type boards[], int id, char desc[], int index);
int update_board(board_type boards[], int id, char desc[], int index);
int archive_board(board_type boards[], int id, int *count);
void display_board(board_type boards[], int id);
int add_card_to_board(board_type boards[], int index, int b_id, int c_id);
int remove_card_from_board(board_type boards[], int index, int b_id, int c_id);

int import_cards(card_type cards[], int *index);
int export_cards(char const filename[], card_type cards[], int index);
int add_card(card_type cards[], int index, int board, int id, char desc[], int user, card_states state);
int update_card(card_type cards[], int index, int id, char desc[], int user, card_states state);
int archive_card(card_type cards[], int id, int *index);
void display_card(card_type cards[], int id);
int add_user_to_card(card_type cards[], int index, int c_id, int u_id);
int remove_user_from_card(card_type cards[], int index, int c_id, int u_id);

int import_users(user_type users[], int *index);
int export_users(char const filename[], user_type users[], int index);
int add_user(user_type users[], int id, char name[], int index);
int update_user(user_type users[], int id, char name[], int index);
int archive_user(user_type users[], int id, int *index);

#endif /* TRELLO_H */

