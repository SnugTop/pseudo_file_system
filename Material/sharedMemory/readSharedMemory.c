//readSharedMemory.c
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
 
const int ARRAY_SIZE = 11;
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
    char readArray[11] = {'\0'};
    
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
    if(addPt == (void *) -1)
    {
    	fprintf(stderr, "Error mmap() failed...: %s\n", strerror(errno));
		return(-1);
    }
    
    smAddPt = (char *)addPt;
    
    // we will read data from shared memory
    // note no synchronization, could read before anything is written
    // we will talk about synchronization later, but we 
    // read 10 bytes
    for(int i = 0; i < 10; i++)
    {
    	readArray[i] = *(smAddPt+i);
    }
    readArray[10] = '\0';
    printf("ProcID[%d]: Read from Shared Memory=: %s\n", (int) getpid(), readArray);
    printf("Sleeping for 20 seconds....\n");
    sleep(20);
    
    // reset and overwrite with digits
    smAddPt = (char *)addPt;
    
    // this time re-read and writing process should have changed the values...
    // to digits

    strncpy(readArray, smAddPt, ARRAY_SIZE-1);
    printf("ProcID[%d]: Read from Shared Memory=: %s\n", (int) getpid(), readArray);
    printf("Sleeping for 20 seconds....\n");
    sleep(20);
    
    // munmap and exitwe can now use smAddPt to modify shared memory
    //
    
    rval = munmap(addPt, pageSize);
    if(rval != 0)
    {
    	fprintf(stderr, "Error munmap() failed...: %s\n", strerror(errno));
		return(-1);
    }
    printf("Reading Shared Memory Completed.... \n"); 
    
    return 0;
}
