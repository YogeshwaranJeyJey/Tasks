#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_SIZE 4096
#define TOTAL_PAGES 64
#define MIN_ALLOC 1024  
#define MAX_ALLOC 8192   

typedef enum {
    FREE,
    PARTIAL,
    FULL
} PageState;

int main() {
    int allocations = 20;
    int page_usage[TOTAL_PAGES] = {0};

    srand(time(NULL));

    for (int i = 0; i < allocations; i++) {
        int alloc_size = MIN_ALLOC + rand() % (MAX_ALLOC - MIN_ALLOC + 1);
        int pages_needed = (alloc_size + PAGE_SIZE - 1) / PAGE_SIZE;

        int start_page = -1;
        for (int j = 0; j <= TOTAL_PAGES - pages_needed; j++) {
            int can_fit = 1;
            for (int k = 0; k < pages_needed; k++) {
                if (page_usage[j + k] > 0) {
                    can_fit = 0;
                    break;
                }
            }
            if (can_fit) {
                start_page = j;
                break;
            }
        }

        if (start_page == -1) {
            printf("Memory fragmented. Allocation of %d bytes failed.\n", alloc_size);
            continue;
        }

        for (int p = 0; p < pages_needed; p++) {
            if (p == pages_needed - 1) {
                int leftover = alloc_size % PAGE_SIZE;
                if (leftover == 0)
                    page_usage[start_page + p] = PAGE_SIZE;
                else
                    page_usage[start_page + p] = leftover;
            } else {
                page_usage[start_page + p] = PAGE_SIZE;
            }
        }
    }

    printf("\nMemory Page Map (64 pages):\n");
    printf("Legend: F = Full, P = Partial, . = Free\n\n");

    for (int i = 0; i < TOTAL_PAGES; i++) {
        PageState state;
        if (page_usage[i] == 0)
            state = FREE;
        else if (page_usage[i] == PAGE_SIZE)
            state = FULL;
        else
            state = PARTIAL;

        switch (state) {
            case FREE: printf("."); break;
            case FULL: printf("F"); break;
            case PARTIAL: printf("P"); break;
        }

        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n");
    return 0;
}
