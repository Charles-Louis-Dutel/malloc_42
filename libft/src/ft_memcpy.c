#include "../includes/libft.h"


void    *ft_memcpy(void *dest, const void *src, size_t n)
{
    if (!dest && !src)
        return (NULL);
    size_t                i = 0; 
    const unsigned char  *s = (const unsigned char *)src;
    unsigned char        *d = (unsigned char *)dest;

    while (i < n)
    {
        d[i] = s[i];
        i++;
    }

    return (dest);
}