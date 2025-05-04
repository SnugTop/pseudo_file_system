#include "disk.h"
#include "inode.h"
#include "dir.h"    
#include <stdio.h>
#include <stdlib.h> 

//use file-system from (b)

//part (h)
int print_files(char* file_name) { //file_name either file1.txt or file2.txt
  
  //print out file 1
  PDOS_FILE * pf1 = pdos_open(file_name, "r");
  char c1 = "";
  unsigned int i1 = 0; //shows how many bytes printed
  char* cc1 = new char[1024]; //what will be outputted
  while (c1 != EOF) {
    c1 = pdos_fgetc(pf1);
    cc1[i1] = c1;
    i1++;
  }
  
  printf("%s/n", cc1); //actual contents
  printf("%d/n", i1); //should be 1024

}
