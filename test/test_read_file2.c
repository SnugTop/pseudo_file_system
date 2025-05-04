#include "disk.h"
#include "inode.h"
#include "dir.h"    
#include <stdio.h>
#include <stdlib.h> 

//part (e) repeating part (d) for file2.txt
//also part (f) in terms of outputting file2.txt 

int read2_funcD() {
  PDOS_FILE * pf1 = pdos_open("file2.txt", "r");
  for (int count = 0; count < 1023; count++) {
    //a char but input as an int
    int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
    int c = pdos_fgetc(c, pf1);
    if (c != c1) {
      printf("/n incorrect on number %d", count+1);
    }
    char cc = char(c);
    printf("%c", cc); //should output EOF too
  }
  char c = char(pdos_fgetc(c, pf1));
  if (c != EOF) {
    printf("/n incorrect on number 1024"); 
  }
  pdos_fclose(pf1);
}

//part (g) in terms of outputting file2.txt

int read2_funcF() {
  PDOS_FILE * pf2 = pdos_open("file2.txt", "r");
  for (int count = 0; count < 1023; count++) {
    //a char but input as an int
    int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
    int c = pdos_fgetc(c, pf2);
    if (c != c1) {
      printf("/n incorrect on number %d", count+1);
    }
    char cc = char(c);
    printf("%c", cc); //should output EOF too
  }
  for (int count = 0; count < 1024; count++) { //indices 1023 - 2046, inclusive
    //a char but input as an int
    int c1 = 'A' + (count % 26); //A-Z, A-Z, etc because starts over
    int c = pdos_fgetc(c, pf2);
    if (c != c1) {
      printf("/n incorrect on number %d", count+1023+1);
    }
    char cc = char(c);
    printf("%c", cc); //should output EOF too
  }
  char c = char(pdos_fgetc(c, pf2));
  if (c != EOF) {
    printf("/n incorrect on number 2048");
  }
  pdos_fclose(pf2);
}
