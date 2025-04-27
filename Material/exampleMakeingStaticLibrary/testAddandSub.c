/*
*   program to test the static library
*   j. montgomery
*
*/

#include <stdio.h>
#include <assert.h>

// include our library header file
#include "addSub.h"

int main(int argc, char *argv[]){

	// create some variables
	int csum = 0;
	int cdiff = 0;

	csum = add(1, 2);

	// test 
	assert((1+2) == csum);
	printf("add() test passed: 1 + 2 == add(1,2) == %d\n", csum);


	// test sub
	cdiff = sub(1,2);
	assert((1-2) == cdiff);
	printf("sub() test passed: 1 - 2 == sub(1,2) == %d\n", cdiff);



 return 0;
}
