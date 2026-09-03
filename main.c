#include "malloc_42.h"
#include "libft.h"


int main()
{
    char *test_large1 = malloc(110001);

    write(1, "Adresse de test_large1: ", 25);  
    ft_put_address_hexa((__uint64_t)test_large1);
    write(1, "\n\n", 3);

    free(test_large1);

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


    write(1, "\n\n", 3);

    write(1, "Test 1er malloc: ", 18);
    write(1, test, ft_strlen(test));
    write(1, "\n", 2);
    write(1, "Adresse de test: ", 18);  
    ft_put_address_hexa((__uint64_t)test);
    write(1, "\n\n", 3);

    write(1, "Test 2e malloc: ", 17);
    write(1, test2, ft_strlen(test2));
    write(1, "\n", 2);
    write(1, "Adresse de test2: ", 19);  
    ft_put_address_hexa((__uint64_t)test2);
    write(1, "\n\n", 3);

    write(1, "Test 3e malloc: ", 17);
    write(1, test3, ft_strlen(test3));
    write(1, "\n", 2);
    write(1, "Adresse de test3: ", 19);  
    ft_put_address_hexa((__uint64_t)test3);
    write(1, "\n\n", 3);

    free(test);
    // write(1, "Test apres free: ", 18);
    // write(1, test, ft_strlen(test));
    // write(1, "Adresse de test: ", 18);  
    // ft_put_address_hexa((__uint64_t)test);
    // write(1, "\n\n", 3);

    char *test4 = malloc(9);

    i = 0;
    while (string[i])
    {
        test4[i] = string[i];
        i++;
    }
    test4[i] = '\0';

    write(1, "Test malloc apres free: ", 18);
    write(1, test4, ft_strlen(test4));
    write(1, "\n", 2);
    write(1, "Adresse de test: ", 18);  
    ft_put_address_hexa((__uint64_t)test4);
    write(1, "\n\n", 3);


    int *int_test = malloc(150000);

    write(1, "Adresse de int_test: ", 22);  
    ft_put_address_hexa((__uint64_t)int_test);
    write(1, "\n\n", 3);

    char *test_large = malloc(120000);

    write(1, "Adresse de test_large: ", 24);  
    ft_put_address_hexa((__uint64_t)test_large);
    write(1, "\n\n", 3);

    free(int_test);

    char *test_large2 = malloc(5003);

    write(1, "Adresse de test_large2: ", 25);  
    ft_put_address_hexa((__uint64_t)test_large2);
    write(1, "\n\n", 3);

    show_alloc_mem();

    free(test_large2);
    free(test_large);
    
    write(1, "\n", 2);
    show_alloc_mem();

    test4 = realloc(test4, 16);
    test3 = realloc(test3, 155);

    write(1, "\n", 2);
    show_alloc_mem();
    return (0);
}