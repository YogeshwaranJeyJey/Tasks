#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_SIZE 4096
#define TOTAL_PAGES 32
#define MIN_ALLOC 1024
#define MAX_ALLOC 8192
#define MAX_BLOCKS 100

typedef enum {
    FREE,
    PARTIAL,
    FULL
} PageState;

typedef struct {
    int id;
    int start_page;
    int pages;
    int valid;
} Block;

int page_usage[TOTAL_PAGES];
int page_owner[TOTAL_PAGES];
Block blocks[MAX_BLOCKS];

int current_block_id = 1;
int block_count = 0;

void init_memory() {
    for (int i = 0; i < TOTAL_PAGES; i++) {
        page_usage[i] = 0;
        page_owner[i] = -1;
    }
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].valid = 0;
    }
}

void print_memory() {
    printf("\nPage Status:\nLegend: F = Full, P = Partial, . = Free\n");

    for (int i = 0; i < TOTAL_PAGES; i++) {
        if (page_owner[i] == -1) {
            printf(".");
        } else if (page_usage[i] == PAGE_SIZE) {
            printf("F");
        } else {
            printf("P");
        }

        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}

int allocate_memory() {
    int alloc_size = MIN_ALLOC + rand() % (MAX_ALLOC - MIN_ALLOC + 1);
    int pages_needed = (alloc_size + PAGE_SIZE - 1) / PAGE_SIZE;

    int start = -1;
    for (int i = 0; i <= TOTAL_PAGES - pages_needed; i++) {
        int fit = 1;
        for (int j = 0; j < pages_needed; j++) {
            if (page_owner[i + j] != -1) {
                fit = 0;
                break;
            }
        }
        if (fit) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        printf("Allocation failed. Memory is fragmented!\n");
        return -1;
    }

    int id = current_block_id++;
    Block *b = &blocks[block_count++];
    b->id = id;
    b->start_page = start;
    b->pages = pages_needed;
    b->valid = 1;

    for (int i = 0; i < pages_needed; i++) {
        int page = start + i;
        if (i == pages_needed - 1) {
            int leftover = alloc_size % PAGE_SIZE;
            page_usage[page] = (leftover == 0) ? PAGE_SIZE : leftover;
        } else {
            page_usage[page] = PAGE_SIZE;
        }
        page_owner[page] = id;
    }

    printf("Allocated Block %d of %d bytes (%d pages)\n", id, alloc_size, pages_needed);
    return id;
}

void free_memory(int block_id) {
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].id == block_id && blocks[i].valid) {
            int start = blocks[i].start_page;
            int pages = blocks[i].pages;
            blocks[i].valid = 0;

            for (int j = 0; j < pages; j++) {
                int page = start + j;
                page_usage[page] = 0;
                page_owner[page] = -1;
            }

            printf("Freed Block %d\n", block_id);
            return;
        }
    }
}

void coalesce_memory() {
    printf("Coalescing adjacent free pages...\n");
}

int get_random_active_block() {
    int attempts = 20;
    while (attempts--) {
        int idx = rand() % block_count;
        if (blocks[idx].valid)
            return blocks[idx].id;
    }
    return -1;
}

int main() {
    srand(time(NULL));
    init_memory();

    for (int i = 0; i < 10; i++) {
        allocate_memory();
        print_memory();
    }

    for (int i = 0; i < 10; i++) {
        int id_to_free = get_random_active_block();
        if (id_to_free != -1) {
            free_memory(id_to_free);
            coalesce_memory();
            print_memory();
        }
        allocate_memory();
        print_memory();
    }

    return 0;
}
