/*
 * Simple allocator and GC implementation
 * http://maplant.com/gc.html
 */

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


/*
 * Scan the free list and look for a place to put the block. 
 * Need to look out from any place where this block could have been 
 * partitioned offf..
 */
static void
add_to_free_list(header_t *bp) {
    header_t *p;
    
    /*
     * Walk to find out the potential point where we could
     * have got the memory from
     */
    for (p = freep; !(bp > p && bp < p->next); p++)
        if (p >= p->next && (bp > p || bp < p->next))
            break;

    if (bp + bp->size == p->next) {
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }

    if (p + p->size == bp) {
        p->size += bp->size;
        p->next = bp->next;
    } else {
        p->next = bp;
    }

    freep = p;
}
