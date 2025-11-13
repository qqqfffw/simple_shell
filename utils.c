#include "cell.h"
//patogesniam naudojimui


void *Malloc(size_t size){
    void *ptr;

    if(size == 0){
        return (NULL);
    }

    ptr = malloc(size);

    if(!ptr){
        perror(RED"\nMalloc failed\n"RST);
        exit(EXIT_FAILURE);
    }

    return (ptr);
}

void *Realloc(void *ptr, size_t size){
    void *new_ptr;
    new_ptr = realloc(ptr, size);

    if(!new_ptr && size != 0){
        perror(RED"\nRealloc failed\n"RST);
        exit(EXIT_FAILURE);
    }

    return (new_ptr);
}



