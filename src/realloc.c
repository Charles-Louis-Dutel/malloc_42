#include "../includes/malloc_42.h"


static void    *checkPtrValidity(void *ptr)
{
    struct Bloc *current_bloc = NULL;
    struct Zone *current_zone = zone;

    while (current_zone)
    {
        current_bloc = current_zone->bloc_list;
        while (current_bloc)
        {
            if ((__uint64_t)current_bloc == (__uint64_t)ptr)
            {
                if (!current_bloc->available)
                    return (ptr);
                return (NULL);
            }
            current_bloc = current_bloc->next;
        }
        current_zone = current_zone->next;
    }
    return (NULL);
}

void    *realloc(void *ptr, size_t size)
{
    if (!ptr)
        return(malloc(size));

    struct Bloc *current = checkPtrValidity((char *)ptr - align16(sizeof(Bloc)));
    
    if (!current)
        return (NULL);
    if (size == 0)
        return(free(ptr), NULL);
    if (size <= current->size)
        return (ptr);

    void    *new_ptr = malloc(size);
    if (!new_ptr)
        return (NULL);
    
    new_ptr = ft_memcpy(new_ptr, ptr, current->data_size);
    if (!new_ptr)
        return (NULL);
    free(ptr);
    return (new_ptr);
}