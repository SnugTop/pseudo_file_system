#include "disk.h"
#include "inode.h"
#include "dir.h"    
#include <stdio.h>
#include <stdlib.h> 

//use file-system from (b)

//part (c) for file1.txt
int write1_funcC() {
  PDOS_FILE * pf1 = pdos_open("file1.txt", "w"); //creates file

  for (int count = 0; count < 1023; count++) { //fills in each in turn
    int c = 'A' + (count % 26); //A-Z, A-Z, etc.
    //a char but input as an int
    pdos_fputc(c, pf1);
  }
  pdos_fclose(pf1);
  return 0;
}

//part (f) for writing to file1
int write1_funcC() {
  PDOS_FILE * pf2 = pdos_open("file1.txt", "rw"); //creates file -- rw to enable both get and put

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
