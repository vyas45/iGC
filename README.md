# iGC
* A Simple Malloc and GC implementation following : http://maplant.com/gc.html


General Design :
==============

* Implementing a simple malloc() before implementing the GC

* malloc():
  ========

  * Maintain a list of free mem blocks that can be partitioned and given out when user needs it.
  * If no blocks available then a bigger block is partitioned or more memory is asked from the kernel
  * free() would just add the chunk to the free list. 
  * Each chunk in free list begins with a header:
      
       typedef struct header {
            unsigned int   size; // Size of the chunk
            struct header* next; // Next free block of memory
       } header_t;
   
   * Maintain a used list to keep track of blocks in use. Items added to used list when removed
     from the free list and vice-versa. 

   * The heap starts at a low addreess from BSS and then grows till the "break". To request more
     memory from the kernel we call the "sbrk" system call to extend the "break" by the argument
     given to the sys call and it returns the pointer to the previous break. 

   * Two functions : morecore() and add_to_free_list()

   * moecore() will be called to request more memory from the kernel. Since the sys calls are 
     expensive, it will be done in page size chunks. Page is the smallest unit of virtual mem
     that can be mapped to phsyical memory. 

  
