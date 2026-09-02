#include "malloc_42.h"
#include "libft.h"


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



    char *test2 = malloc(1523);
    char string2[152] = "sdagfadfafsdfsadfasdfadff4s5adf5d4fas85ed4fasd5f4sad5f";

    i = 0;
    while (string2[i])
    {
        test2[i] = string2[i];
        i++;
    }
    test2[i] = '\0';



    char *test3 = malloc(51);
    char string3[45] = "test3 cavavavava";

    i = 0;
    while (string3[i])
    {
        test3[i] = string3[i];
        i++;
    }
    test3[i] = '\0';



    write(1, "Test 1er malloc: ", 18);
    write(1, test, ft_strlen(test));
    write(1, "\n\n", 3);
    write(1, "Test 2e malloc: ", 17);
    write(1, test2, ft_strlen(test2));
    write(1, "\n\n", 3);
    write(1, "Test 3e malloc: ", 17);
    write(1, test3, ft_strlen(test3));
    return (0);
}