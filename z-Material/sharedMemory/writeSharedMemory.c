//writeSharedMemory.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * Read data from shared memory object
 * 	assume shared memory object already created
 * 	We assume that there is one page of shared memory
 */
 

int main(int argc, char** argv) {

    /*
     * 
     * 
     */
     
    int pageSize = -1;
    char *smAddPt = NULL;
    void *addPt = NULL;
    int shmFH = -1;
    int rval = -1;
    char character = 'a'; 

    
    pageSize = sysconf(_SC_PAGESIZE);
    printf("System Page size = %d\n", pageSize);
  
    /*
    * get File descriptor for shared memory object.
    */
    shmFH = shm_open("/shmTest", O_RDWR , 0666 );
    if(shmFH == -1){
		fprintf(stderr, "Could not open Shared Memory File...: %s\n", strerror(errno));
		return(-1);
    }
    
    // We have a file descriptor, mmap it into our virtual memory address space
    
    addPt = mmap(NULL, pageSize, PROT_READ| PROT_WRITE, MAP_SHARED, shmFH, 0);
    if( addPt == (void *) -1)
    {
    	fprintf(stderr, "Error mmap() failed...: %s\n", strerror(errno));
		return(-1);
    }
    
    smAddPt = (char *)addPt;
    while (character <= 'j')
    {
    	*smAddPt = character;
    	smAddPt++; character++;
    }
    printf("ProcID[%d] Wrote \"abcdefghij\" to shared memory. \n", (int) getpid());
    printf("ProcID[%d] Sleeping for 20 seconds...\n", (int) getpid());
    sleep(20);
    
    // reset and overwrite with digits
    smAddPt = (char *)addPt;
    character = '0';
    
    printf("ProcID[%d] Overwriting \"abcdefghij\" with \"0123456789\" in shared memory. \n", (int) getpid());
    printf("ProcID[%d] Sleeping for 20 seconds...\n", (int) getpid());
    while (character <= '9')
    {
    	*smAddPt = character;
    	smAddPt++; character++;
    }
    sleep(20);
    
    // munmap and exit
    //
    
    rval = munmap(addPt, pageSize);
    if(rval != 0)
    {
    	fprintf(stderr, "Error munmap() failed...: %s\n", strerror(errno));
		return(-1);
    }
    printf("Writing to Shared Memory Completed \n"); 
    
    return 0;
}
