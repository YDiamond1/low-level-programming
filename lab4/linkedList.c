//
// Created by YDiamond on 03.11.2020.
//
#include <stdio.h>
#include <malloc.h>
#include "linkedList.h"


llist_node* llist_create(int number){
        llist_node *tmp = malloc(sizeof(llist_node));
        tmp->prev = NULL;
        tmp->value = number;
        tmp->next = NULL;
        return tmp;
}

llist_node* list_add_front (llist_node** llist, int number){
    llist_node* front = llist_create(number);
    if(*llist == NULL){
        *llist = front;
    }
    else{
        front->next = *llist;
        (*llist)->prev = front;
        *llist = front;
    }

    return *llist;
}
llist_node* list_add_back (llist_node** llist, int number){
    llist_node* back = llist_create(number);
    if(*llist == NULL){
        *llist = back;
    }
    else{
        llist_node* iter = *llist;
        while (iter->next != NULL) iter = iter->next;
        iter->next = back;
        back->prev = iter;
    }
    return back;
}
llist_node* llist_at(llist_node* llist, int at, int* error){
    if(length(llist)==0 || at<0){
        *error = 1;

        return NULL;
    }
    llist_node* iter = llist;
    for(; at >= 0 ; --at, iter = iter->next){
        if(iter == NULL){
            *error = 1;
            return NULL;
        }
    }
    return iter;
}
int list_get (llist_node* list, int index, int* error){
    llist_node* at = llist_at(list, index, error);
    if(*error == 1){
        return 0;
    }
    return at->value;
}

int length(llist_node* llist){
    int length = 0;
    for(;llist != NULL; llist = llist->next)
        length++;
    return length;
}
static void print_int(int number){
    printf("%d ", number);
}
int sum(llist_node* iter){
    int sum = 0;
    for(;iter != NULL; iter = iter->next) {
        sum += iter->value;
    }
    return sum;
}

void llist_free (llist_node* llist){
    while(llist != NULL){
        llist_node* next = llist->next;
        free(llist);
        llist = next;
    }
}
