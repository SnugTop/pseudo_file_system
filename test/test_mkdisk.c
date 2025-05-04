#include "disk.h"
#include <stdio.h>

int main() {
    if (pdos_mkdisk() == 0) {
        printf("Disk created successfully.\n");
    } else {
        printf("Disk creation failed.\n");
        return 1;
    }

    if (pdos_mkfs("MYFS2025") == 0) {
        printf("Filesystem formatted successfully.\n");
    } else {
        printf("Filesystem format failed.\n");
        return 1;
    }

    //parts c through h of the assignment code to write:
    write1_funcC(); //c
    read1_funcD(); //d 
    write2_funcC(); //e--repeat c
    read2_funcD(); //e--repeat d
    write1_funcF(); //f--write
    read1_funcF(); //f--read 1
    read2_funcD(); //f--read 2
    write2_funcF(); //g--write
    read1_funcF(); //g--read 1
    read2_funcF(); //g--read 2
    print_files("file1.txt"); //h--file 1
    print_files("file2.txt"); //h--file 2
    
    return 0;
}
