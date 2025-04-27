//
/**
 * @file union.c
 * @brief Demonstrates simple usage of union in C.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

struct ThreeValues_I
{
    int a;
    int b;
    int c;
};

struct ThreeValuesS
{
    char a[12];
    char b[12];
    char c[12];
};

struct ThreeValues_D
{
    double a;
    double b;
    double c;
};

int main(int argc, char const *argv[])
{
    struct ThreeValues_I var1 = {.a = 1, .b = 2, .c = 3};
    struct ThreeValues_I var1_1 = {.a = 101, .b = 203, .c = 303};
    printf("size of ThreeValues_I: %zu\n", sizeof(var1));

    // create a ThreeValuesS structure
    struct ThreeValuesS var2 = {.a = "hello,", .b = "COSC 1030", .c = "!"};
    printf("size of ThreeValuesS: %zu\n", sizeof(var2));

    // create a ThreeValues_D structure
    struct ThreeValues_D var3 = {.a = 1.0, .b = 2.0, .c = 3.0};
    printf("size of ThreeValues_D: %zu\n", sizeof(var3));

    int fd = open("struct_data_var_size.bin", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    int bytes__wrote = write(fd, &var1, sizeof(var1));
    printf("wrote %d\n", bytes__wrote);

     bytes__wrote = write(fd, &var2, sizeof(var2));
    printf("wrote %d\n", bytes__wrote);

     bytes__wrote = write(fd, &var3, sizeof(var3));
    printf("wrote %d\n", bytes__wrote);

    bytes__wrote = write(fd, &var1_1, sizeof(var1_1));
    printf("wrote %d\n", bytes__wrote);

    close(fd);
    return 0;
}