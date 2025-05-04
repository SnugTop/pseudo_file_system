#include "disk.h"
#include "inode.h"
#include "dir.h"    
#include <stdio.h>
#include <stdlib.h> 

//use file-system from (b)

//part (c) for file1.txt
PDOS_FILE * pf = pdos_open("file1.txt", "w"); //creates file

for (int count = 0; count < 1023; count++) {
  int c = 'A' + (count % 26); //A-Z, A-Z, etc.
  //a char but input as an int
  pdos_fputc(c, pf);
}
pdos_fclose(pf);

//part (d) for file1.txt

PDOS_FILE * pf1 = pdos_open("file1.txt", "r");
for (int count = 0; count < 1024; count++) {
  //a char but input as an int
  int c = pdos_fgetc(c, pf);
  char cc = char(c);
  printf("%c", cc); //should output EOF too
}
pdos_fclose(pf1);
