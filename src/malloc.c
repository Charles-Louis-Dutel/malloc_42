#include "../includes/malloc_42.h"

struct Zone *zone = NULL;

void    *malloc(size_t size)
{
    write(1, "MALLOC PERSO\n", 14);
    int     zone_type_needed;
    size_t  zone_size;
    
    zone_type_needed = getZoneType(size);
    if (zone_type_needed == SIZE_ERROR)
    {
        write(1, "Error\n", 7); // A changer mettre en write 2
        return (NULL);
    }

    //Si il n'y a pas encore de zone, on en cree une 
    if (!zone)
    {
        zone_size = getZoneSize(zone_type_needed, size);
        if (zone_size == 0)
        {
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
        newzone->bloc_list = createNewBlocNode((char *)newzone + align16(sizeof(Zone)), size, NULL);
        zone = newzone;

        write(1, "Passe dans !zone\n", 18);
        return ((char *)newzone->bloc_list + align16(sizeof(Bloc)));
    }
    write(1, "Passe pas dans !zone\n", 22);
    return (NULL);
    // struct Zone *temp_zone = zone;
    






    // while (temp_zone)
    // {
    //     if (!temp_zone->zone_type)
    //     {
    //         temp_zone->zone_type = zone_type_needed;
    //         temp_zone->size = zone_size;
    //         temp_zone->next = NULL;
    //         //Creer fonction pour creer le bloc: temp_zone->bloc_list = createNewBlocNode(size);
    //         return (NULL); //A modifier, doit return l'adresse du debut de l'espace data creer dans le bloc
    //     }
    //     else if (temp_zone->zone_type && temp_zone->zone_type != zone_type_needed)
    //         temp_zone = temp_zone->next;    //Mauvais type de zone
    //     else
    //     {
    //         struct Bloc *temp_bloc = temp_zone->bloc_list;

    //         while (temp_bloc)
    //         {
    //             if (temp_bloc->available)
    //             {
    //                 //Checker size dispo

    //                 //Si ok Bloc a return
    //                 return (NULL); //temporaire
    //             }
    //             temp_bloc = temp_bloc->next;
    //         }
    //         temp_zone = temp_zone->next;
    //     }
    // }
}