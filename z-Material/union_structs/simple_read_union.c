//simple_read_union.c
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

    const int BOX_SIZE = sizeof(union ThreeCommon);
    union ThreeCommon tmp = {0};
    // read 4th block, skip 3-blocks
    int skip_pos = BOX_SIZE * 3;
    int rtn_val = lseek(fd, skip_pos, SEEK_SET);

    rtn_val = read(fd, &tmp, BOX_SIZE);
    printf("Last struct(ThreeValues_I) a: %d, b: %d, c: %d\n", tmp.iv.a, tmp.iv.b, tmp.iv.c);

    // seek and read 2nd block (strings)
    skip_pos = 1 * BOX_SIZE;
    rtn_val = lseek(fd, skip_pos, SEEK_SET);

    rtn_val = read(fd, &tmp, BOX_SIZE);
    printf("2nd BLOCK - Type: ThreeValues_S:: a: %s, b: %s, c: %s\n",
                                    tmp.sv.a, tmp.sv.b, tmp.sv.c);



    return 0;
}