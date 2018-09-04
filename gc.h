#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> // sbrk()

/*
 * Each memory block header
 */
typedef struct header {
    unsigned int  size;
    struct header *next;
} header_t;


static header_t base;            /* First zero sized block */
static header_t *freep = &base;  /* Pointer to first free block of memory */
static header_t *usedp;          /* Pointer to first used block of memory */

#define MIN_ALLOC_SIZE 4096      /* As we allocate the memory in page sized chunks */ 



