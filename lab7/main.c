#include <stdio.h>
#include "mem.h"
int main() {
    char* str = mem_alloc(2000);
    puts("1v"); memalloc_debug_heap(stdout);
    char* str2 = mem_alloc(2000);
    puts("2v"); memalloc_debug_heap(stdout);
    char* str3 = mem_alloc(2000);
    puts("3v"); memalloc_debug_heap(stdout);

    scanf("%s", str);
    printf("%s\n", str);
    puts("test str"); memalloc_debug_heap(stdout);

    mem_free(str);
    puts("1f"); memalloc_debug_heap(stdout);
    mem_free(str2);
    puts("2f"); memalloc_debug_heap(stdout);
    mem_free(str3);
    puts("3f"); memalloc_debug_heap(stdout);
}
