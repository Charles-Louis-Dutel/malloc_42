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

    //final_zone_size = (((size + sizeof(Zone)) * 100 / page_size) + 1) * page_size;

    if (size <= SMALL_BLOCK)
        final_zone_size = (sizeof(Zone) + ((size + sizeof(Bloc)) * 100));
    else
        final_zone_size = (sizeof(Zone) + size + sizeof(Bloc));

    if ((final_zone_size % page_size != 0))
        final_zone_size = (final_zone_size / page_size * page_size) + page_size;

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
    int     zone_type_needed;
    size_t  zone_size;
    
    zone_type_needed = getZoneType(size);
    if (zone_type_needed == SIZE_ERROR)
    {
        //error message
        return (NULL);
    }

    //Si il n'y a pas encore de zone, on en cree une 
    if (!zone)
    {
        zone_size = getZoneSize(zone_type_needed, size);
        if (zone_size == -1)
        {
            return (NULL);
        }

        unsigned prot;
        
        prot |= (1 << PROT_READ | 1 << PROT_WRITE);
        struct Zone *newzone = mmap(NULL, zone_size, prot, MAP_ANONYMOUS, -1, 0);

        zone = newzone;
    }

    struct Zone *temp_zone = zone;
    
    while (temp_zone)
    {
        if (!temp_zone->zone_type)
        {
            temp_zone->zone_type = zone_type_needed;
            temp_zone->size = zone_size;
            temp_zone->next = NULL;
            //Creer fonction pour creer le bloc: temp_zone->bloc_list = createNewBlocNode(size);
            return (NULL); //A modifier, doit return l'adresse du debut de l'espace data creer dans le bloc
        }
        else if (temp_zone->zone_type && temp_zone->zone_type != zone_type_needed)
            temp_zone = temp_zone->next;    //Mauvais type de zone
        else
        {
            struct Bloc *temp_bloc = temp_zone->bloc_list;

            while (temp_bloc)
            {
                if (temp_bloc->available)
                {
                    //Checker size dispo

                    //Si ok Bloc a return
                    return (NULL); //temporaire
                }
                temp_bloc = temp_bloc->next;
            }
            temp_zone = temp_zone->next;
        }
    }
}