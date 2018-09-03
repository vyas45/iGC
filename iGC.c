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
