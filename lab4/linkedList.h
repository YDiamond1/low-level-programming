//
// Created by YDiamond on 03.11.2020.
//

#ifndef LABA4_LINKEDLIST_H
#define LABA4_LINKEDLIST_H



typedef struct llist_node {
    struct llist_node *next;
    struct llist_node *prev;
    int value;
} llist_node;


llist_node* llist_create(int);
llist_node* list_add_front (llist_node** llist, int number);
llist_node* list_add_back (llist_node** llist, int number);
llist_node* llist_at(llist_node* llist, int at, int* error);
int list_get (llist_node* list, int index, int* error);
int length(llist_node* llist);
int sum(llist_node* llist);
void llist_free (llist_node* llist);
#endif //LABA4_LINKEDLIST_H
