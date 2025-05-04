#include "disk.h"
#include "inode.h"
#include "dir.h"    
#include <stdio.h>
#include <stdlib.h> 

//use file-system from (b)

//copy part (c) in part (e) for file2.txt
int write1_funcC() {
  PDOS_FILE * pf1 = pdos_open("file2.txt", "w"); //creates file

  for (int count = 0; count < 1023; count++) { //fills in each in turn
    int c = 'A' + (count % 26); //A-Z, A-Z, etc.
    //a char but input as an int
    pdos_fputc(c, pf1);
  }
  pdos_fclose(pf1);
  return 0;
}

//part (g) for writing to file2
int write1_funcC() {
  PDOS_FILE * pf2 = pdos_open("file2.txt", "rw"); //creates file

  //skips through already-written bytes
  for (int count = 0; count < 1023; count++) { //fills in each in turn
    //a char but input as an int
    pdos_getc(c, pf2);
  }

  //newly-written bytes
  for (int count = 0; count < 1024; count++) { //fills in each in turn
    int c = 'A' + (count % 26); //A-Z, A-Z, etc.
    //a char but input as an int
    pdos_fputc(c, pf2);
  }

  //close file
  pdos_fclose(pf2);
  return 0;
}
