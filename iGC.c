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


/*
 * THE Allocator
 *
 * Scan the free list and find the block that is
 * as big as the amount of memory requested
 *
 * As we return the "first" block that we find 
 * this is "first-fit" allocation
 *
 * Note: the size is in header-sized blocks and not
 * bytes
 *
 */
void* 
GC_malloc(size_t alloc_size) {
    size_t num_units;
    header_t *p, *prevp;

    num_units = (alloc_size + sizeof(header_t) - 1)/sizeof(header_t)  + 1;
    prevp = freep;

    for (p = prevp->next;; prevp = p, p = p->next) {
        /* Potential big-enough block */
        if (p->size >= num_units) {
            /* Exact size block */
            if (p->size == num_units) {
                prevp->next = p->next;
            } else {
                p->size -= num_units;
                p += p->size;
                p->size = num_units;
            }

            freep = prevp;

            /* Add p to the used list */
            if (usedp == NULL)
                usedp = p->next = p;
            else {
                p->next = usedp->next;
                usedp->next = p;
            }

            return (void *) (p+1);
        }

        /* Not enough memory */
        if (p == freep) {
            p = morecore(num_units);
            if (p == NULL)
                return (NULL);
        }
    }
}

int main() {
    return 0;
}
    
