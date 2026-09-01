#include "../includes/malloc_42.h"


int main()
{
    char *test = malloc(7);

    char string[7] = "prout";

    int i = 0;
    while (string[i])
    {
        test[i] = string[i];
        i++;
    }
    test[i] = '\0';

    printf("Test 1er malloc: %s", test);

    return (0);
}