#include "../includes/malloc_42.h"


size_t  align16(size_t  size)
{
    size_t alignment = 0;

    alignment = size + (16 - size % 16);
    return (alignment);
}