// test/for_submission.c
// What we are supposed to submit
#include "fs.h"
#include <stdio.h>
#include <stdbool.h>

//this is the 

int main() {
    // create and format a fresh disk
    // part A: one that creates a disk file system
    printf("/n short program A/n");
    if (pdos_mkdisk(true) == 0) {
        printf("Disk created successfully.\n");
        //since ftruncate() is called, if the size is not DISK_SIZE (1024*1091) then an error will occur
        //this does not warrant a specific printing
    } else {
        printf("Disk creation failed.\n");
        return 1;
    }
    //  part B: one that formats and initializes the disk by writing the FS structure
    //  again, nothing to output
    printf("/n short program B/n");
    if (pdos_mkfs("MYFS2025") == 0) {
        printf("Filesystem formatted successfully.\n");
    } else {
        printf("Filesystem format failed.\n");
        return 1;
    }
    //  part C: one that uses the above FS and creates a test file(file1.txt) in the root directory
    printf("/n short program C/n");
    PDOS_FILE * pf1 = pdos_open("file1.txt", "w"); //creates file
    //nothing actually written
    for (int count = 0; count < 1023; count++) { //fills in each in turn
      int c = 'A' + (count % 26); //A-Z, A-Z, etc.
      //a char but input as an int
      pdos_fputc(c, pf1);
    }
    pdos_fclose(pf1);
  
    //  part D: one that reads in the data from above file and prints it out ensuring it matches
    printf("/n short program D/n");
    PDOS_FILE * pf2 = pdos_open("file1.txt", "r"); //only needs to read
  
    for (int count = 0; count < 1023; count++) { //goes through each in turn
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
      int c = pdos_fgetc(pf2);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1);
      }
    }
  
    char c = char(pdos_fgetc(pf2)); //EOF
    if (c != EOF) {
      printf("/n incorrect on number 1024/n"); //for last EOF at end
    }
    pdos_fclose(pf2); //close pointer

    //  part E: Repeat (c,d), this time create a second file file2.txt
    printf("/n short program E/n");
  
    //  E replicating C:
    printf("/n short program E replicating C/n");
    PDOS_FILE * pf3 = pdos_open("file2.txt", "w"); //creates file

    for (int count = 0; count < 1023; count++) { //fills in each in turn
      int c = 'A' + (count % 26); //A-Z, A-Z, etc.
      //a char but input as an int
      pdos_fputc(c, pf3);
    }
    pdos_fclose(pf3);
  
    //  E replicating D:
    printf("/n short program E replicating D/n");
    PDOS_FILE * pf4 = pdos_open("file2.txt", "r");
    for (int count = 0; count < 1023; count++) {
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
      int c = pdos_fgetc(pf4);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1);
      }
    }
    char c = char(pdos_fgetc(pf4)); //EOF
    if (c != EOF) {
      printf("/n incorrect on number 1024/n"); 
    }
    pdos_fclose(pf4);

    //part F:
    printf("/n short program F/n");
    PDOS_FILE * pf5 = pdos_open("file1.txt", "rw"); //creates file -- rw to enable both get and put

    //skips through already-written bytes
    for (int count = 0; count < 1023; count++) { //fills in each in turn
      //a char but input as an int
      pdos_getc(pf5);
    }

    //newly-written bytes
    for (int count = 0; count < 1024; count++) { //fills in each in turn
      int c = 'A' + (count % 26); //A-Z, A-Z, etc.
      //a char but input as an int
      pdos_fputc(c, pf5);
    }

    //close file
    pdos_fclose(pf5);
    //  F replicating D -- checking file1
    printf("/n short program F replicating D for file1/n");
    PDOS_FILE * pf6 = pdos_open("file1.txt", "r");
    for (int count = 0; count < 1023; count++) {
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
      int c = pdos_fgetc(pf6);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    for (int count = 0; count < 1024; count++) { //indices 1023 - 2046, inclusive
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc because starts over
      int c = pdos_fgetc(pf6);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1023+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    char c = char(pdos_fgetc(pf6));
    if (c != EOF) {
      printf("/n incorrect on number 2048/n");
    }
    pdos_fclose(pf6);

    //  F replicating D -- checking file2
    printf("/n short program F replicating D for file2/n");
    PDOS_FILE * pf7 = pdos_open("file2.txt", "r");
    for (int count = 0; count < 1023; count++) {
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
      int c = pdos_fgetc(pf7);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    char c = char(pdos_fgetc(pf7));
    if (c != EOF) {
      printf("/n incorrect on number 1024/n"); 
    }
    pdos_fclose(pf7);

    //part G:
    printf("/n short program G/n");
    PDOS_FILE * pf8 = pdos_open("file2.txt", "rw"); //creates file

    //skips through already-written bytes
    for (int count = 0; count < 1023; count++) { //fills in each in turn
      //a char but input as an int
      pdos_getc(pf8);
    }

    //newly-written bytes
    for (int count = 0; count < 1024; count++) { //fills in each in turn
      int c = 'A' + (count % 26); //A-Z, A-Z, etc.
      //a char but input as an int
      pdos_fputc(c, pf8);
    }

    //close file
    pdos_fclose(pf8);

    //  G replicating D -- checking file1
    printf("/n short program G replicating D for file2/n");
    printf("/n short program F replicating D for file1/n");
    PDOS_FILE * pf9 = pdos_open("file1.txt", "r");
    for (int count = 0; count < 1023; count++) {
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
      int c = pdos_fgetc(pf9);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    for (int count = 0; count < 1024; count++) { //indices 1023 - 2046, inclusive
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc because starts over
      int c = pdos_fgetc(pf9);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1023+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    char c = char(pdos_fgetc(pf9));
    if (c != EOF) {
      printf("/n incorrect on number 2048/n");
    }
    pdos_fclose(pf9);
  
    //  G replicating D -- checking file2
    printf("/n short program G replicating D for file2/n");
    PDOS_FILE * pf0 = pdos_open("file2.txt", "r");
    for (int count = 0; count < 1023; count++) {
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc.
      int c = pdos_fgetc(pf0);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    for (int count = 0; count < 1024; count++) { //indices 1023 - 2046, inclusive
      //a char but input as an int
      int c1 = 'A' + (count % 26); //A-Z, A-Z, etc because starts over
      int c = pdos_fgetc(pf0);
      if (c != c1) {
        printf("/n incorrect on number %d/n", count+1023+1);
      }
      char cc = char(c);
      printf("%c", cc); //should output EOF too
    }
    char c = char(pdos_fgetc(pf0));
    if (c != EOF) {
      printf("/n incorrect on number 2048/n");
    }
    pdos_fclose(pf0);
  
    //part H file 1:
    printf("/n short program H for file 1/n");
    PDOS_FILE * pfA = pdos_open(file_name, "r");
    char c1 = "";
    unsigned int i1 = 0; //shows how many bytes printed
    char* cc1 = new char[1024]; //what will be outputted
    while (c1 != EOF) {
      c1 = pdos_fgetc(pfA);
      cc1[i1] = c1;
      i1++;
    }
  
    printf("%s/n", cc1); //actual contents
    printf("%d/n", i1); //should be 1024

    pdos_close(pfA);
  
    //part H file 2:
    printf("/n short program H for file 2/n");
    PDOS_FILE * pfB = pdos_open(file_name, "r");
    char c1 = "";
    unsigned int i1 = 0; //shows how many bytes printed
    char* cc1 = new char[1024]; //what will be outputted
    while (c1 != EOF) {
      c1 = pdos_fgetc(pfB);
      cc1[i1] = c1;
      i1++;
    }
  
    printf("%s/n", cc1); //actual contents
    printf("%d/n", i1); //should be 1024

    pdos_close(pfB);
    
    //part I:
    printf("/n short program I/n");
    char** ccs = pdos_dir();
    for (short count; count < size(ccs) / 12; count++) {
        //divide by 12 because max name length is 12 and that's what's stored
        printf("%s/n", ccs[count]);
    }
  
    return 0;
}
