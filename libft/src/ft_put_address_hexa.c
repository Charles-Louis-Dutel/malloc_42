#include "../includes/libft.h"


void    ft_put_address_hexa(__uint64_t address)
{
    char        address_in_hexa[17];
    __uint8_t   to_convert;
    int         i = 0;
    
    if (address == 0)
        return (ft_putstr("0x0"));
    while (address > 0)
    {
        to_convert = address & 0xF;
        address_in_hexa[i] = HEXA[to_convert];
        i++;
        address = address >> 4;
    }
    address_in_hexa[i] = '\0';
    i--;

    ft_putstr("0x");
    while (i >= 0)
    {
        ft_putchar(address_in_hexa[i]);
        i--;
    }
}