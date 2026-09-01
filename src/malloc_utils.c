#include "../includes/malloc_42.h"


int  getZoneType(size_t size)
{
    if (size <= 0)
        return (SIZE_ERROR);
    if (size <= TINY_BLOC)
        return (TINY_TYPE);
    else if (size > TINY_BLOC && size <= SMALL_BLOCK)
        return (SMALL_TYPE);
    return (LARGE_TYPE);
}


long   getPageSize(size_t size)
{
    long  page_size;
    
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size < 0)
    {
        //error message
        return (-1);
    }

    size_t final_zone_size;

    //final_zone_size = (((size + sizeof(Zone)) * 100 / page_size) + 1) * page_size;

    if (size <= SMALL_BLOCK)
        final_zone_size = (align16(sizeof(Zone)) + ((size + align16(sizeof(Bloc))) * 100));
    else
        final_zone_size = (align16(sizeof(Zone)) + size + align16(sizeof(Bloc)));

    if ((final_zone_size % page_size != 0))
        final_zone_size = (final_zone_size / page_size * page_size) + page_size;

    return (final_zone_size);
}


size_t   getZoneSize(int zone_type, size_t size)
{
    if (zone_type == TINY_TYPE)
        return (getPageSize(TINY_BLOC));
    else if (zone_type == SMALL_TYPE)
        return (getPageSize(SMALL_BLOCK));
    return (getPageSize(size));
}


size_t  align16(size_t  size)
{
    size_t alignment = 0;

    alignment = size + (16 - size % 16);
    return (alignment);
}


struct Bloc *createNewBlocNode(void *addr, size_t size, struct Bloc *prevBloc)
{
    struct Bloc  *newbloc = addr;

    newbloc->size = align16(size);
    newbloc->available = false;
    newbloc->next = NULL;
    newbloc->prev = prevBloc;

    return (newbloc);
}