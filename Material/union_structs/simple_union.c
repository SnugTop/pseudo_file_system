//simple_union.c
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
union ThreeCommon{
    struct ThreeValues_I iv; 
    struct ThreeValues_S sv; 
    struct ThreeValues_D dv; 
}; 

int main(int argc, char const *argv[])
{
    // open file for writing
    int fd = open("simple_union_data_uniform_size.bin", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);

    // create a common type capable of holding any of the three data types
    union ThreeCommon tmp={0};

    printf("size of ThreeCommon: %ld\n", sizeof(tmp));

    // init with I
    tmp.iv.a = 10;
    tmp.iv.b = 20;
    tmp.iv.c = 30;

    int bytes_wrote = write(fd, &tmp, sizeof(tmp));

    // init with S
    memset(&tmp, 0, sizeof(tmp));
    strcpy(tmp.sv.a, "Hello");
    strcpy(tmp.sv.b, "COSC-1030");
    strcpy(tmp.sv.c, "Yahoo!");
    bytes_wrote = write(fd, &tmp, sizeof(tmp));

    // init with D
    memset(&tmp, 0, sizeof(tmp));
    tmp.dv.a = 1.15;
    tmp.dv.b = 2.25;
    tmp.dv.c = 3.35;
    bytes_wrote = write(fd, &tmp, sizeof(tmp));

    // last another iv
    memset(&tmp, 0, sizeof(tmp));
    tmp.iv.a = 101;
    tmp.iv.b = 202;
    tmp.iv.c = 303;
    bytes_wrote = write(fd, &tmp, sizeof(tmp));
    close(fd);
  

    return 0;
}