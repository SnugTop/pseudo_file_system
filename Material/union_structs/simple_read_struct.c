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

struct ThreeValues_S
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
    struct ThreeValues_I var1; 
    //struct ThreeValues_D var3;
    struct ThreeValues_S var2;

    int fd = open("struct_data_var_size.bin", O_RDWR, S_IRUSR | S_IWUSR);

    // read last ThreeValues_I from file
    // skip over (ThreeValues_I + ThreeValues_S + ThreeValues_D)
    // each a different size...
    int skip_pos = sizeof(struct ThreeValues_I) + sizeof(struct ThreeValues_S) + sizeof(struct ThreeValues_D) ;
    int rtn_val = lseek(fd, skip_pos, SEEK_SET);

    rtn_val = read(fd, &var1, sizeof(struct ThreeValues_I));
    printf("a: %d, b: %d, c: %d\n", var1.a, var1.b, var1.c);
    close(fd);

    printf("size of ThreeValuesS: %zu\n", sizeof(var2));

    
    return 0;
}