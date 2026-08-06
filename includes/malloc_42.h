#ifndef MALLOC_42_H
# define MALLOC_42_H
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>

#define SIZE_ERROR 0
#define TINY_TYPE 1
#define SMALL_TYPE 2
#define LARGE_TYPE 3

#define TINY_BLOC 64
#define SMALL_BLOCK 1024
// #define TINY_ZONE 12288
// #define SMALL_ZONE 106496

typedef struct Bloc {
    int             size;
    bool            available;
    struct Bloc     *prev;
    struct Bloc     *next;
} Bloc;


typedef struct Zone {
    int             zone_type;
    int             size;
    struct Bloc     *bloc_list;
    struct Zone     *next;
} Zone;


struct Zone         *zone;


void                free(void *ptr);
void                *malloc(size_t size);
void                *realloc(void *ptr, size_t size);