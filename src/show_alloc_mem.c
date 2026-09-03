#include "../includes/malloc_42.h"


void    show_alloc_mem(void)
{
    struct Zone *current_zone = zone;
    struct Bloc *current_bloc = NULL;
    char        zone_size[3][6] = {"TINY", "SMALL", "LARGE"};

    while (current_zone)
    {
        ft_putstr(zone_size[current_zone->zone_type - 1]);
        ft_putstr(" : ");
        ft_put_address_hexa((__uint64_t) current_zone);
        ft_putchar('\n');

        current_bloc = current_zone->bloc_list;
        while (current_bloc)
        {
            if (!current_bloc->available)
            {
                ft_put_address_hexa((__uint64_t)((char *)current_bloc + align16(sizeof(Bloc))));
                ft_putstr(" - ");
                ft_put_address_hexa((__uint64_t)((char *)current_bloc + align16(sizeof(Bloc)) + current_bloc->data_size));
                ft_putstr(" : ");
                ft_putnbr(current_bloc->data_size);
                ft_putstr(" bytes\n");
            }
            current_bloc = current_bloc->next;
        }
        current_zone = current_zone->next;
    }
}