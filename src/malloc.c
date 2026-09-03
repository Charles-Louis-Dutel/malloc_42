#include "../includes/malloc_42.h"

struct Zone *zone = NULL;


static struct Bloc *createNewBlocNode(void *addr, size_t size, struct Bloc *prev_bloc, struct Bloc *next_bloc)
{
    struct Bloc  *newbloc = addr;

    newbloc->data_size = size;
    newbloc->size = align16(size);
    newbloc->available = false;
    newbloc->next = next_bloc;
    newbloc->prev = prev_bloc;

    return (newbloc);
}


static struct Zone *createNewZoneNode(int zone_type_needed, size_t bloc_size)
{
    size_t  zone_size;

    zone_size = getZoneSize(zone_type_needed, bloc_size);
    if (zone_size == 0)
        return (NULL);

    unsigned prot = PROT_READ | PROT_WRITE;

    struct Zone *newzone = mmap(NULL, zone_size, prot, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (newzone == MAP_FAILED)
        return (NULL);

    newzone->zone_type = zone_type_needed;
    newzone->size = zone_size;
    newzone->next = NULL;
    newzone->bloc_list = createNewBlocNode((char *)newzone + align16(sizeof(Zone)), bloc_size, NULL, NULL);

    return (newzone);
}


void    *malloc(size_t size)
{
    //write(1, "MALLOC PERSO\n\n", 15);
    int     zone_type_needed;
    
    zone_type_needed = getZoneType(size);
    if (zone_type_needed == SIZE_ERROR)
        return (NULL);

    if (!zone)
    {
        zone = createNewZoneNode(zone_type_needed, size);
        if (!zone)
            return (NULL);
        return ((char *)zone->bloc_list + align16(sizeof(Bloc)));
    }

    struct Zone *temp_zone = zone;
    struct Zone *prev_zone = NULL;

    while (temp_zone)
    {
        if (temp_zone->zone_type != zone_type_needed || zone_type_needed == LARGE_TYPE)
        {
            prev_zone = temp_zone;
            temp_zone = temp_zone->next;
        }
        else
        {
            struct Bloc *temp_bloc = temp_zone->bloc_list;
            struct Bloc *prev_bloc = NULL; 

            while (temp_bloc)
            {
                if (temp_bloc->available)
                {
                    if (temp_bloc->size >= align16(size))
                    {
                        temp_bloc->available = false;
                        temp_bloc->data_size = size;
                        return ((char *)temp_bloc + align16(sizeof(Bloc)));
                    }
                }
                prev_bloc = temp_bloc;
                temp_bloc = temp_bloc->next;
            }

            if ((size_t)(((char *)temp_zone + temp_zone->size) -
                ((char *)prev_bloc + align16(sizeof(Bloc)) + prev_bloc->size)) >= align16(sizeof(Bloc)) + align16(size))
            {
                temp_bloc = createNewBlocNode((char *)prev_bloc + align16(sizeof(Bloc)) + prev_bloc->size, size, prev_bloc, NULL);
                prev_bloc->next = temp_bloc;
                return ((char *)temp_bloc + align16(sizeof(Bloc)));
            }
            prev_zone = temp_zone;
            temp_zone = temp_zone->next;
        }
    }

    struct Zone *newzone = createNewZoneNode(zone_type_needed, size);
    if (!newzone)
        return (NULL);

    if ((__uint64_t)newzone > (__uint64_t)prev_zone)
        prev_zone->next = newzone;
    else
    {
        temp_zone = zone;
        prev_zone = NULL;

        if ((__uint64_t)temp_zone > (__uint64_t)newzone)
        {
            newzone->next = temp_zone;
            zone = newzone;
        }
        else
        {
            while (temp_zone)
            {
                if ((__uint64_t)temp_zone->next > (__uint64_t)newzone)
                {
                    newzone->next = temp_zone->next;
                    temp_zone->next = newzone;
                    break;
                }
                temp_zone = temp_zone->next;
            }
        }
    }
    return ((char *)newzone->bloc_list + align16(sizeof(Bloc)));
}