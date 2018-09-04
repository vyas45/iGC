/*
 * Simple allocator and GC implementation
 * http://maplant.com/gc.html
 */

#include "gc.h"

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


/*
 * Ask for more memory from the kernel using the sbrk() sys call
 * If the requested page size is less than our minimum page
 * size then we pump up the requested size to match the page. 
 * Once we get the page(s) we add it to the free list
 */
static header_t*
morecore(size_t num_units) {
    void *vp;
    header_t *up;

    if (num_units < MIN_ALLOC_SIZE)
        num_units = MIN_ALLOC_SIZE;

    if ((vp = sbrk(num_units * sizeof(header_t))) == (void *) -1)
        return (NULL);

    up = (header_t *)vp;
    up->size = num_units;
    add_to_free_list(up);
    return freep;
}


int main() {
    return 0;
}
    
