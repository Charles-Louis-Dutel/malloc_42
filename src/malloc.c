#include "../includes/malloc_42.h"



static int  getZoneType(size_t size)
{
    if (size <= 0)
        return (SIZE_ERROR);
    if (size <= TINY_BLOC)
        return (TINY_TYPE);
    else if (size > TINY_BLOC && size <= SMALL_BLOCK)
        return (SMALL_TYPE);
    return (LARGE_TYPE);
}


static long   getPageSize(size_t size)
{
    long  page_size;
    
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size < 0)
    {
        //error message
        return (-1);
    }

    size_t final_zone_size;

    final_zone_size = (((size + sizeof(Zone)) * 100 / page_size) + 1) * page_size;
    return (final_zone_size);
}


static size_t   getZoneSize(int zone_type, size_t size)
{
    if (zone_type == TINY_TYPE)
        return (getPageSize(TINY_BLOC));
    else if (zone_type == SMALL_TYPE)
        return (getPageSize(SMALL_BLOCK));
    return (getPageSize(size));
}


void    *malloc(size_t size)
{
    if (!zone)
    {
        int zone_type;
        
        zone_type = getZoneType(size);
        if (zone_type == SIZE_ERROR)
        {
            //error message
            return (NULL);
        }

        size_t zone_size;

        zone_size = getZoneSize(zone_type, size);
        if (zone_size == -1)
        {
            return (NULL);
        }

        unsigned prot;
        
        prot |= (1 << PROT_READ | 1 << PROT_WRITE);
        struct Zone *newzone = mmap(NULL, zone_size, prot, MAP_ANONYMOUS, -1, 0);
    }
}