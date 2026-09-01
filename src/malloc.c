#include "../includes/malloc_42.h"

struct Zone *zone = NULL;


static struct Bloc *createNewBlocNode(void *addr, size_t size, struct Bloc *prevBloc)
{
    struct Bloc  *newbloc = addr;

    newbloc->size = align16(size);
    newbloc->available = false;
    newbloc->next = NULL;
    newbloc->prev = prevBloc;

    return (newbloc);
}


static struct Zone *createNewZoneNode(int zone_type_needed, size_t bloc_size)
{
    size_t  zone_size;

    zone_size = getZoneSize(zone_type_needed, bloc_size);
    if (zone_size == 0)
    {
        write(1, "zoneSize error\n", 16);
        return (NULL);
    }

    unsigned prot = 0;

    prot |= PROT_READ | PROT_WRITE;
    struct Zone *newzone = mmap(NULL, zone_size, prot, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (newzone == MAP_FAILED)
    {
        write(1, "mmap error\n", 12);
        return (NULL);
    }

    newzone->zone_type = zone_type_needed;
    newzone->size = zone_size;
    newzone->next = NULL;
    newzone->bloc_list = createNewBlocNode((char *)newzone + align16(sizeof(Zone)), bloc_size, NULL);

    return (newzone);
}


void    *malloc(size_t size)
{
    write(1, "MALLOC PERSO\n", 14);

    int     zone_type_needed;
    
    zone_type_needed = getZoneType(size);
    if (zone_type_needed == SIZE_ERROR)
    {
        write(1, "Error\n", 7); // A changer mettre en write 2
        return (NULL);
    }

    //Si il n'y a pas encore de zone, on en cree une 
    if (!zone)
    {
        write(1, "Passage si zone n'existe pas encore\n", 37);
        // zone_size = getZoneSize(zone_type_needed, size);
        // if (zone_size == 0)
        // {
        //     write(1, "zoneSize error\n", 16);
        //     return (NULL);
        // }

        // unsigned prot = 0;
        
        // prot |= PROT_READ | PROT_WRITE;
        // struct Zone *newzone = mmap(NULL, zone_size, prot, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        // if (newzone == MAP_FAILED)
        // {
        //     write(1, "mmap error\n", 12);
        //     return (NULL);
        // }

        // newzone->zone_type = zone_type_needed;
        // newzone->size = zone_size;
        // newzone->next = NULL;
        // newzone->bloc_list = createNewBlocNode((char *)newzone + align16(sizeof(Zone)), size, NULL);
        zone = createNewZoneNode(zone_type_needed, size);
        if (!zone)
            return (NULL);
        return ((char *)zone->bloc_list + align16(sizeof(Bloc)));
    }


    struct Zone *temp_zone = zone;
    struct Zone *prev_zone = NULL;

    while (temp_zone)
    {
        if (temp_zone->zone_type && temp_zone->zone_type != zone_type_needed)
        {
            write(1, "Zone non correspondante, passage zone suivante\n", 48);
            prev_zone = temp_zone;
            temp_zone = temp_zone->next;
        }
        else
        {
            struct Bloc *temp_bloc = temp_zone->bloc_list;
            struct Bloc *prev_bloc = NULL; 

            while (temp_bloc)
            {
                write(1, "Passage dans 1 bloc\n", 21);
                if (temp_bloc->available)
                {
                    write(1, "Bloc disponible\n", 17);
                    if (temp_bloc->size >= size)
                    {
                        write(1, "Bloc disponible et taille du bloc assez grande\n", 48);
                        temp_bloc->available = false;
                        return ((char *)temp_bloc + align16(sizeof(Bloc)));
                    }
                }
                prev_bloc = temp_bloc;
                temp_bloc = temp_bloc->next;
            }

            if ((size_t)(((char *)temp_zone + temp_zone->size) - (char *)temp_bloc) >= size)
            {
                write(1, "Creation d'un bloc\n", 20);
                write(1, "Adresse temp_bloc: ", 20);
                

                temp_bloc = createNewBlocNode(temp_bloc, size, prev_bloc);
                write(1, "Prev bloc bug ?\n", 17);
                prev_bloc->next = temp_bloc;
                write(1, "Pas de bug Prev bloc\n", 22);
                return ((char *)temp_bloc + align16(sizeof(Bloc)));
            }
            prev_zone = temp_zone;
            temp_zone = temp_zone->next;
        }
    }
    write(1, "Creation d'une zone\n", 21);
    temp_zone = createNewZoneNode(zone_type_needed, size);
    if (!temp_zone)
        return (NULL);
    prev_zone->next = temp_zone;
    return ((char *)temp_zone->bloc_list + align16(sizeof(Bloc)));
}