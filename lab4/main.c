#include <stdio.h>
#include "linkedList.h"
#include <malloc.h>

static void print_int(int);
static void print_newline();
static void print_list(llist_node** list){
    llist_node* iter = *list;

    for(;iter!=NULL; iter= iter->next){
        print_int(iter->value);
    }
    print_newline();
}
static void print_int(int number){
    printf("%d ", number);
}
static void print_newline(){
    printf("\n");
}
static void print(char* string){
    printf("%s", string);
}
int main() {
    int value;

    llist_node** list = malloc(sizeof(llist_node**));
    *list = NULL;

    print("Enter your numbers: \0");
    while(scanf("%d", &value) == 1){
        list_add_front(list, value);
    }
    if(*list != NULL) {
        print("input have ended\n\0");

        print("Your list is \0");
        print_list(list);
        print("The length is \0");
        print_int(length(*list));
        print_newline();

        print("Sum is \0");
        print_int(sum(*list));
        print_newline();

        int *error = malloc(sizeof(int));
        *error = 0;
        int n = list_get(list, length(*list) - 1, error);
        if (!(*error)) {
            print("The n-th element is \0");
            print_int(n);
        } else {
            print("List is empty ");
        }
    }else{
        print("list is empty\0");
    }

    llist_free(*list);
    return 0;
}