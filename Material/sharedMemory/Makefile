all: createSharedMemory readSharedMemory writeSharedMemory

createSharedMemory: createSharedMemory.c
	gcc -g -Wall -o createSharedMemory createSharedMemory.c -lrt

readSharedMemory: readSharedMemory.c
	gcc -g -Wall -o readSharedMemory readSharedMemory.c -lrt

writeSharedMemory: writeSharedMemory.c
	gcc -g -Wall -o writeSharedMemory writeSharedMemory.c -lrt

clean:
	rm createSharedMemory
	rm readSharedMemory
	rm writeSharedMemory

