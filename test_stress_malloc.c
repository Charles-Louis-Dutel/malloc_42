#include "malloc_42.h"
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/*
** Gros programme de test pour malloc / free / realloc / show_alloc_mem.
**
** Compile par exemple avec ta lib :
**   cc -Wall -Wextra -Werror test_stress_malloc.c -L. -lft_malloc -o test_stress_malloc
**
** Puis :
**   ./run.sh ./test_stress_malloc
**
** IMPORTANT :
** - Les tests évitent printf pour ne pas provoquer d'allocations parasites.
** - Certains comportements invalides (double free, free d'un pointeur inconnu)
**   ne sont PAS testés comme exigences normales : en C, ils sont indéfinis.
*/

static int g_tests = 0;
static int g_failed = 0;

static size_t s_len(const char *s)
{
    size_t n = 0;

    while (s && s[n])
        n++;
    return (n);
}

static void putstr(const char *s)
{
    if (s)
        write(1, s, s_len(s));
}

static void putnbr_size(size_t n)
{
    char c;

    if (n >= 10)
        putnbr_size(n / 10);
    c = (char)('0' + (n % 10));
    write(1, &c, 1);
}

static void puthex_uintptr(uintptr_t n)
{
    const char *base = "0123456789abcdef";
    char buf[2 + sizeof(uintptr_t) * 2];
    int i = (int)sizeof(buf) - 1;

    if (n == 0)
    {
        putstr("0x0");
        return;
    }
    while (n)
    {
        buf[i--] = base[n % 16];
        n /= 16;
    }
    buf[i--] = 'x';
    buf[i] = '0';
    write(1, &buf[i], sizeof(buf) - (size_t)i);
}

static void section(const char *name)
{
    putstr("\n\n============================================================\n");
    putstr(name);
    putstr("\n============================================================\n");
}

static void result(int ok, const char *name)
{
    g_tests++;
    if (ok)
        putstr("[OK]   ");
    else
    {
        putstr("[FAIL] ");
        g_failed++;
    }
    putstr(name);
    putstr("\n");
}

static void info_ptr(const char *name, const void *ptr)
{
    putstr("       ");
    putstr(name);
    putstr(" = ");
    puthex_uintptr((uintptr_t)ptr);
    putstr("\n");
}

static int pattern_ok(const unsigned char *p, size_t n, unsigned char seed)
{
    size_t i = 0;

    while (i < n)
    {
        if (p[i] != (unsigned char)(seed + (i % 251)))
            return (0);
        i++;
    }
    return (1);
}

static void fill_pattern(unsigned char *p, size_t n, unsigned char seed)
{
    size_t i = 0;

    while (i < n)
    {
        p[i] = (unsigned char)(seed + (i % 251));
        i++;
    }
}

static void test_basic_mallocs(void)
{
    void *p1;
    void *p2;
    void *p3;
    void *p4;
    void *p5;
    void *p6;

    section("1. MALLOC BASIQUE + FRONTIERES");

    p1 = malloc(1);
    result(p1 != NULL, "malloc(1)");
    if (p1)
        ((unsigned char *)p1)[0] = 0x11;

    p2 = malloc(16);
    result(p2 != NULL, "malloc(16)");
    if (p2)
    {
        fill_pattern((unsigned char *)p2, 16, 1);
        result(pattern_ok((unsigned char *)p2, 16, 1),
            "ecriture/lecture des 16 octets");
    }

    p3 = malloc(64);
    result(p3 != NULL, "malloc(64) - limite TINY presumee");
    if (p3)
        fill_pattern((unsigned char *)p3, 64, 2);

    p4 = malloc(65);
    result(p4 != NULL, "malloc(65) - juste apres TINY");
    if (p4)
        fill_pattern((unsigned char *)p4, 65, 3);

    p5 = malloc(1024);
    result(p5 != NULL, "malloc(1024) - limite SMALL presumee");
    if (p5)
        fill_pattern((unsigned char *)p5, 1024, 4);

    p6 = malloc(1025);
    result(p6 != NULL, "malloc(1025) - juste apres SMALL / LARGE");
    if (p6)
    {
        fill_pattern((unsigned char *)p6, 1025, 5);
        result(pattern_ok((unsigned char *)p6, 1025, 5),
            "ecriture/lecture allocation LARGE");
    }

    info_ptr("malloc(1)", p1);
    info_ptr("malloc(16)", p2);
    info_ptr("malloc(64)", p3);
    info_ptr("malloc(65)", p4);
    info_ptr("malloc(1024)", p5);
    info_ptr("malloc(1025)", p6);

    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
    free(p6);
}

static void test_free_reuse(void)
{
    unsigned char *a;
    unsigned char *b;
    unsigned char *c;
    uintptr_t old_addr;

    section("2. FREE + REUTILISATION DE BLOCS");

    a = malloc(7);
    result(a != NULL, "malloc(7)");
    if (!a)
        return;

    fill_pattern(a, 7, 10);
    old_addr = (uintptr_t)a;
    free(a);

    b = malloc(9);
    result(b != NULL, "malloc(9) apres free(malloc(7))");
    if (b)
    {
        info_ptr("ancienne adresse", (void *)old_addr);
        info_ptr("nouvelle adresse", b);
        putstr("       meme adresse : ");
        putstr(((uintptr_t)b == old_addr) ? "oui\n" : "non (pas forcement un bug)\n");
        fill_pattern(b, 9, 20);
        result(pattern_ok(b, 9, 20), "bloc reutilise reste inscriptible");
    }

    c = malloc(51);
    result(c != NULL, "malloc(51) apres reutilisation");
    if (c)
    {
        fill_pattern(c, 51, 30);
        result(pattern_ok(c, 51, 30), "allocation voisine non corrompue");
    }

    free(b);
    free(c);
    free(NULL);
    result(1, "free(NULL) ne crash pas");
}

static void test_many_tiny(void)
{
    unsigned char *ptrs[220];
    size_t sizes[220];
    size_t i;
    int ok = 1;

    section("3. BEAUCOUP DE TINY (plus de 100 allocations)");

    i = 0;
    while (i < 220)
    {
        sizes[i] = (i % 63) + 1;
        ptrs[i] = malloc(sizes[i]);
        if (!ptrs[i])
        {
            ok = 0;
            break;
        }
        fill_pattern(ptrs[i], sizes[i], (unsigned char)i);
        i++;
    }
    result(ok, "220 malloc TINY");

    if (ok)
    {
        i = 0;
        while (i < 220)
        {
            if (!pattern_ok(ptrs[i], sizes[i], (unsigned char)i))
            {
                ok = 0;
                break;
            }
            i++;
        }
        result(ok, "contenu des 220 TINY intact");
    }

    i = 0;
    while (i < 220)
    {
        if (ptrs[i])
            free(ptrs[i]);
        i++;
    }
}

static void test_many_small(void)
{
    unsigned char *ptrs[140];
    size_t sizes[140];
    size_t i;
    int ok = 1;

    section("4. BEAUCOUP DE SMALL (plus de 100 allocations)");

    i = 0;
    while (i < 140)
    {
        sizes[i] = 65 + (i % 900);
        ptrs[i] = malloc(sizes[i]);
        if (!ptrs[i])
        {
            ok = 0;
            break;
        }
        ptrs[i][0] = (unsigned char)i;
        ptrs[i][sizes[i] - 1] = (unsigned char)(i + 1);
        i++;
    }
    result(ok, "140 malloc SMALL");

    if (ok)
    {
        i = 0;
        while (i < 140)
        {
            if (ptrs[i][0] != (unsigned char)i
                || ptrs[i][sizes[i] - 1] != (unsigned char)(i + 1))
            {
                ok = 0;
                break;
            }
            i++;
        }
        result(ok, "premier/dernier octet des SMALL intacts");
    }

    i = 0;
    while (i < 140)
    {
        if (ptrs[i])
            free(ptrs[i]);
        i++;
    }
}

static void test_large_free(void)
{
    unsigned char *a;
    unsigned char *b;
    unsigned char *c;

    section("5. LARGE + MUNMAP");

    a = malloc(5003);
    b = malloc(120000);
    c = malloc(250000);

    result(a != NULL, "malloc LARGE 5003");
    result(b != NULL, "malloc LARGE 120000");
    result(c != NULL, "malloc LARGE 250000");

    if (a)
    {
        a[0] = 'A';
        a[5002] = 'Z';
    }
    if (b)
    {
        b[0] = 'B';
        b[119999] = 'Y';
    }
    if (c)
    {
        c[0] = 'C';
        c[249999] = 'X';
    }

    putstr("\n--- show_alloc_mem AVANT free des LARGE ---\n");
    show_alloc_mem();

    free(b);
    putstr("\n--- show_alloc_mem APRES free du LARGE milieu ---\n");
    show_alloc_mem();

    free(a);
    free(c);

    putstr("\n--- show_alloc_mem APRES free de tous ces LARGE ---\n");
    show_alloc_mem();
}

static void test_realloc_null(void)
{
    unsigned char *p;

    section("6. REALLOC(NULL, size)");

    p = realloc(NULL, 128);
    result(p != NULL, "realloc(NULL, 128) se comporte comme malloc");
    if (p)
    {
        fill_pattern(p, 128, 40);
        result(pattern_ok(p, 128, 40), "memoire retournee par realloc(NULL, 128) utilisable");
        free(p);
    }
}

static void test_realloc_grow(void)
{
    uint64_t old_addr;
    unsigned char *p;
    unsigned char *q;
    size_t old_size = 32;
    size_t new_size = 400;

    section("7. REALLOC AGRANDISSEMENT");

    p = malloc(old_size);
    result(p != NULL, "malloc avant realloc grow");
    if (!p)
        return;

    fill_pattern(p, old_size, 50);
    old_addr = (uint64_t)p;
    q = realloc(p, new_size);

    result(q != NULL, "realloc 32 -> 400");
    if (!q)
        return;

    result(pattern_ok(q, old_size, 50),
        "les 32 anciens octets sont conserves");
    fill_pattern(q + old_size, new_size - old_size, 60);
    result(pattern_ok(q + old_size, new_size - old_size, 60),
        "nouvelle partie est inscriptible");

    info_ptr("avant realloc", (void *)old_addr);
    info_ptr("apres realloc", q);

    free(q);
}

static void test_realloc_shrink(void)
{
    uint64_t old_addr;
    unsigned char *p;
    unsigned char *q;

    section("8. REALLOC REDUCTION");

    p = malloc(500);
    result(p != NULL, "malloc(500) avant shrink");
    if (!p)
        return;

    fill_pattern(p, 500, 70);
    old_addr = (uint64_t)p;
    q = realloc(p, 23);

    result(q != NULL, "realloc 500 -> 23");
    if (!q)
        return;

    result(pattern_ok(q, 23, 70),
        "les 23 premiers octets sont conserves");

    info_ptr("avant realloc", (void *)old_addr);
    info_ptr("apres realloc", q);

    free(q);
}

static void test_realloc_cross_classes(void)
{
    unsigned char *p;
    unsigned char *q;
    unsigned char *r;

    section("9. REALLOC ENTRE TINY / SMALL / LARGE");

    p = malloc(50);
    result(p != NULL, "malloc(50)");
    if (!p)
        return;

    fill_pattern(p, 50, 80);

    q = realloc(p, 700);
    result(q != NULL, "realloc TINY 50 -> SMALL 700");
    if (!q)
        return;
    result(pattern_ok(q, 50, 80), "contenu conserve TINY -> SMALL");

    r = realloc(q, 7000);
    result(r != NULL, "realloc SMALL 700 -> LARGE 7000");
    if (!r)
        return;
    result(pattern_ok(r, 50, 80), "contenu initial conserve SMALL -> LARGE");

    q = realloc(r, 20);
    result(q != NULL, "realloc LARGE 7000 -> TINY 20");
    if (!q)
        return;
    result(pattern_ok(q, 20, 80), "contenu conserve LARGE -> TINY");

    free(q);
}

static void test_realloc_zero_observation(void)
{
    unsigned char *p;
    unsigned char *q;

    section("10. REALLOC(ptr, 0) - OBSERVATION");

    p = malloc(32);
    result(p != NULL, "malloc(32) avant realloc(ptr, 0)");
    if (!p)
        return;

    p[0] = 42;
    q = realloc(p, 0);

    putstr("       retour realloc(ptr, 0) = ");
    puthex_uintptr((uintptr_t)q);
    putstr("\n");
    putstr("       Ce test est informatif : verifier le comportement demande par ton sujet/man.\n");

    if (q)
        free(q);
}

static void test_show_alloc_mem_mixed(void)
{
    void *a;
    void *b;
    void *c;
    void *d;
    void *e;
    void *f;

    section("11. SHOW_ALLOC_MEM - MELANGE + ORDRE D'ADRESSES");

    a = malloc(7);
    b = malloc(9000);
    c = malloc(300);
    d = malloc(51);
    e = malloc(120000);
    f = malloc(700);

    result(a && b && c && d && e && f, "creation d'un melange TINY/SMALL/LARGE");

    putstr("\n--- Affichage : verifier TINY/SMALL/LARGE et adresses croissantes ---\n");
    show_alloc_mem();

    free(c);
    free(b);

    putstr("\n--- Apres free d'un SMALL et d'un LARGE ---\n");
    show_alloc_mem();

    free(a);
    free(d);
    free(e);
    free(f);
}

static void test_fragmentation_and_reuse(void)
{
    unsigned char *p[12];
    size_t i;
    int ok = 1;

    section("12. FRAGMENTATION / REUTILISATION");

    i = 0;
    while (i < 12)
    {
        p[i] = malloc(32 + i);
        if (!p[i])
        {
            ok = 0;
            break;
        }
        fill_pattern(p[i], 32 + i, (unsigned char)(100 + i));
        i++;
    }
    result(ok, "12 allocations consecutives");

    i = 0;
    while (i < 12)
    {
        if ((i % 2) == 0 && p[i])
        {
            free(p[i]);
            p[i] = NULL;
        }
        i++;
    }

    putstr("\n--- Etat apres liberation d'un bloc sur deux ---\n");
    show_alloc_mem();

    i = 0;
    while (i < 6)
    {
        unsigned char *x = malloc(20 + i);
        if (!x)
            ok = 0;
        else
        {
            fill_pattern(x, 20 + i, (unsigned char)(150 + i));
            free(x);
        }
        i++;
    }
    result(ok, "nouvelles petites allocations dans un espace fragmente");

    i = 0;
    while (i < 12)
    {
        if (p[i])
            free(p[i]);
        i++;
    }
}

static void test_1024_x_1024(void)
{
    unsigned char *p[1024];
    size_t i;
    int ok = 1;

    section("13. TEST PROCHE DE LA FICHE : 1024 x malloc(1024)");

    i = 0;
    while (i < 1024)
    {
        p[i] = malloc(1024);
        if (!p[i])
        {
            ok = 0;
            break;
        }

        /*
        ** Toucher au moins un octet de chaque page/allocation.
        ** Ici on ecrit aussi au dernier octet pour verifier les bornes.
        */
        p[i][0] = (unsigned char)i;
        p[i][1023] = (unsigned char)(i >> 8);
        i++;
    }

    result(ok, "1024 allocations de 1024 octets");

    if (ok)
    {
        i = 0;
        while (i < 1024)
        {
            if (p[i][0] != (unsigned char)i
                || p[i][1023] != (unsigned char)(i >> 8))
            {
                ok = 0;
                break;
            }
            i++;
        }
        result(ok, "donnees des 1024 allocations intactes");
    }

    i = 0;
    while (i < 1024)
    {
        if (p[i])
            free(p[i]);
        i++;
    }

    result(1, "1024 free effectues sans crash");
}

static void summary(void)
{
    section("RESUME");
    putstr("Tests effectues : ");
    putnbr_size((size_t)g_tests);
    putstr("\nEchecs detectes : ");
    putnbr_size((size_t)g_failed);
    putstr("\n");

    if (g_failed == 0)
        putstr("\n>>> AUCUN ECHEC DETECTE PAR CE MAIN <<<\n");
    else
        putstr("\n>>> IL RESTE DES TESTS EN ECHEC <<<\n");
}

int main(void)
{
    test_basic_mallocs();
    test_free_reuse();
    test_many_tiny();
    test_many_small();
    test_large_free();

    test_realloc_null();
    test_realloc_grow();
    test_realloc_shrink();
    test_realloc_cross_classes();
    test_realloc_zero_observation();

    test_show_alloc_mem_mixed();
    test_fragmentation_and_reuse();
    test_1024_x_1024();

    summary();
    return (g_failed != 0);
}
