/* 
 * File:   trello.h
 * Author: joshmc@uab.edu
 *
 * Created on August 30, 2022
 */

#ifndef TRELLO_H
#define TRELLO_H

#define list_count_max 8
#define card_count_max 100
#define user_count_max 10

typedef enum {
    low, medium, high, unknown
} priority_level;

typedef struct {
    int list_id;
    char list_desc[50];
    int list_cards[20];
} list_type;

typedef struct {
    int card_id;
    char card_desc[50];
    priority_level card_priority;
    int card_users[4];
} card_type;

typedef struct {
    int user_id;
    char user_name[20];
    int user_level;
} user_type;

void help(void);

int import_lists(list_type lists[], int *index);
int export_lists(char const filename[], list_type lists[], int index);
int add_list(list_type lists[], int index, int id, char desc[]);
int update_list(list_type lists[], int index, int id, char desc[]);
int archive_list(list_type lists[], int *index, int id);
void display_list(list_type lists[], card_type cards[], user_type users[], int id);
int add_card_to_list(list_type lists[], int index, int b_id, int c_id);
int remove_card_from_list(list_type lists[], int index, int b_id, int c_id);

int import_cards(card_type cards[], int *index);
int export_cards(char const filename[], card_type cards[], int index);
int add_card(card_type cards[], int index, int id, char desc[], int user, priority_level priority);
int update_card(card_type cards[], int index, int id, char desc[], priority_level priority);
int archive_card(card_type cards[], int *index, int id);
void display_card(card_type cards[], user_type users[], int id);
int add_user_to_card(card_type cards[], int index, int c_id, int u_id);
int remove_user_from_card(card_type cards[], int index, int c_id, int u_id);

int import_users(user_type users[], int *index);
int export_users(char const filename[], user_type users[], int index);
int add_user(user_type users[], int index, int id, char name[]);
int update_user(user_type users[], int index, int id, char name[]);
int archive_user(user_type users[], int *index, int id);
void display_user(user_type users[], int id);

#endif /* TRELLO_H */

