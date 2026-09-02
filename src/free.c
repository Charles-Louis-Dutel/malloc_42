#include "../includes/malloc_42.h"


void    free(void *ptr)
{
    write(1, "FREE PERSO\n\n", 13);

    if(!ptr)
        return;
        
    struct Bloc *bloc = (struct Bloc *)((char *)ptr - align16(sizeof(Bloc)));

    if (bloc->prev == NULL)
    {
        struct Zone *ptr_zone = (struct Zone *)((char *)bloc - align16(sizeof(Zone)));
        
        if (ptr_zone->zone_type == LARGE_TYPE)
        {
            if (ptr_zone == zone && ptr_zone->next != NULL)
                zone = ptr_zone->next;
            else if (ptr_zone == zone && ptr_zone->next == NULL)
            {
                munmap(ptr_zone, ptr_zone->size);
                zone = NULL;
                return;  
            }
            else
            {
                struct Zone *prev_zone = NULL;
                struct Zone *current_zone = zone;

                while (current_zone && current_zone != ptr_zone)
                {
                    prev_zone = current_zone;
                    current_zone = current_zone->next;
                }
                if (current_zone == ptr_zone)
                    prev_zone->next = current_zone->next;
            }

            munmap(ptr_zone, ptr_zone->size);
            return;
        }
    }
    bloc->available = true;
}