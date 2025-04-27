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
 * Create one page of shared memory
 *  	shared memory will be used to communicate among processes
 * 	if shared memory already exist, return ERROR
 *	Size of page is system dependent (typically 4096 bytes)
 */
 

int main(int argc, char** argv) {

    /*
     * Create a Page of Shared Memory
     * 
     */
     
    int pageSize = -1;
    
    int shmFH = -1;
    int rval = -1;

    
    pageSize = sysconf(_SC_PAGESIZE);
    printf("System Page size = %d\n", pageSize);
  
    /*
    * create shared memory object. If it already exists return error
    */
    shmFH = shm_open("/shmTest", O_RDWR | O_CREAT | O_EXCL, 0666 );

    if(shmFH == -1){
		  fprintf(stderr, "File System create failed: %s\n", strerror(errno));
		  return(-1);
    }
    
    // set size of shared memory to one page
    // should
    rval = ftruncate(shmFH, pageSize);

    if(rval != 0){
    	fprintf(stderr, "Error Setting size of shared  memory : %s\n", strerror(errno));
		  return(-1);
    }
    printf("Shared Memory successfully created\n"); 
    printf("\n\nLook in /dev/shm for shmTest\n");
    
    return 0;
}
