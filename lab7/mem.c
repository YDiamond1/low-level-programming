//
// Created by YDiamond on 03.12.2020.
//

#include "mem.h"

static struct mem_node *heap_start = NULL;

void block_split(struct mem_node *block, uint64_t query_size);
void *page_init(void* page_address, uint64_t page_size,  bool fixed);
bool block_seq(struct mem_node *block, struct mem_node *next);
void block_merge(struct mem_node *block, struct mem_node *next);
bool block_splitable(struct mem_node* block, uint64_t query_size){
    return block->capacity   >= query_size + sizeof(struct mem_node) + BLOCK_MIN_SIZE;
}

void *mem_alloc(uint64_t query){
    struct mem_node* iter, *prev, *retval;
    uint64_t page_size;
    void* address_page_end;

    if(heap_start == NULL) heap_init(HEAP_PAGE_SIZE);
    if(query == 0 ) return NULL;
    if(query < BLOCK_MIN_SIZE) query = BLOCK_MIN_SIZE;

    for(iter = heap_start; iter != NULL; iter = iter->next){
        prev = iter;

        if(!(iter -> is_free)) continue;

        if(!block_splitable(iter,query))
            continue;
        else{
            block_split(iter,query);
            iter -> is_free = false;
            return (char*) iter + sizeof(struct mem_node);
        }
    }

    page_size = query + sizeof(struct mem_node);
    if(page_size < HEAP_PAGE_SIZE) page_size = HEAP_PAGE_SIZE;
    address_page_end = (char*) prev + sizeof(struct mem_node) + prev -> capacity;
    retval = page_init(address_page_end, page_size,true);
    if(retval == MAP_FAILED){
        retval = page_init(address_page_end, page_size, false);
        if(retval == MAP_FAILED) return NULL;
    }
    prev -> next = retval;
    retval -> capacity = page_size - sizeof(struct mem_node);
    retval -> is_free = false;
    retval -> next = NULL;

    if(block_splitable(retval, query)) block_split(retval, query);

    return (char*)retval + sizeof(struct mem_node);
}
void mem_free(void* mem){
    struct mem_node* deleting = (struct mem_node*) ((char*)mem - sizeof(struct mem_node));
    struct mem_node* iter, *next;

    if(mem == NULL) return;
    if(deleting != heap_start){
        for(iter = heap_start; iter -> next != NULL && iter -> next != deleting ; iter = iter->next);
        if(iter -> next != deleting) return;
    }
    deleting -> is_free = true;

    next = deleting->next;
    if(next && next->is_free && block_seq(deleting, next)) block_merge(deleting, next);

}

void block_merge(struct mem_node *block, struct mem_node *next) {
    block -> capacity += next->capacity + sizeof(struct mem_node);
    block -> next = next -> next;
}

bool block_seq(struct mem_node *block, struct mem_node *next) {
    return (struct mem_node*)((char*)block + sizeof(struct mem_node) + block->capacity) == next;
}

void block_split(struct mem_node *block, uint64_t query_size) {
    struct mem_node* new_block = (struct mem_node*)((char*)block + sizeof(struct mem_node) + query_size);
    new_block -> capacity = block->capacity - sizeof(struct mem_node) - query_size;
    new_block -> next = block -> next;
    new_block -> is_free = true;

    block -> capacity = query_size;
    block -> next = new_block;
}

void *page_init( void* page_address, uint64_t page_size,  bool fixed){
    return mmap( page_address,
                 page_size,
                PROT_WRITE | PROT_READ,
                MAP_ANONYMOUS | MAP_PRIVATE | (fixed ?  MAP_FIXED_NOREPLACE : 0),
                -1,
                0);
}
void *heap_init(uint64_t size){
    if(size < HEAP_PAGE_SIZE) size = HEAP_PAGE_SIZE;
    heap_start = page_init(HEAP_START, HEAP_PAGE_SIZE, false);
    heap_start->capacity = size - sizeof(struct mem_node);
    heap_start->next = NULL;
    heap_start->is_free = true;
    return heap_start;
}


void memalloc_debug_struct_info(FILE *f, struct mem_node const * const address) {
    uint64_t i;
    char *ptr = (char*) address + sizeof(struct mem_node);

    fprintf(f, "start: %p, size: %4lu, is_free: %d, bytes:", (void*) address, address->capacity, address->is_free);
    for (i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; i++, ptr++) {
        fprintf(f, " %2hhX", *ptr);
    }
    putc('\n', f);
}

void memalloc_debug_heap(FILE *f) {
    struct mem_node *ptr = heap_start;
    for ( ; ptr; ptr = ptr->next) memalloc_debug_struct_info(f, ptr);
}
