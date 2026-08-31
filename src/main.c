#include "../includes/malloc_42.h"
#include <stdio.h>


int main()
{
    size_t  size = 0;

    size = sizeof(Bloc);
    printf("Taille d'une struct Bloc: %lu\nTaille de size avant align: %lu\n", sizeof(Bloc), size);

    size = align16(size);
    printf("Taille de size apres align: %lu\n", size);
    return (0);
}