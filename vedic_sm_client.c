#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "vedic_sm_shared_stuff.h"

void main( void )
{ 
	int running = 1;
	int *shm = NULL;
	struct shared_stuff_st *memo;
	int n1;
	int shmid;
	key_t key;
	key = 1234;
	
	//int shmget(key_t key, size_t size, int shmflg );
	if ( ( shmid = shmget( key, 1024, IPC_CREAT | 0666 ) ) == -1 )
	{ 
		perror( "shmget");
		exit(1);
	}
	

	if ( ( shm = shmat(shmid, NULL, 0 ) ) == (int*) -1)
	{ 
		perror( "shmat" );
		exit(1);
	}
	
	printf( "Memory attached at %p\n", (int*) shm);
	memo = (struct shared_stuff_st*) shm;
	
	while( running )
	{ 
		memo -> written_by_you = 0;
		
		printf("Enter 2 digit number to square:\t");
		scanf("%d", &n1);
		memo -> numToSq = n1;
		strncpy( memo->some_text, "end", 3);
		memo -> written_by_you = 1;
		if( strncmp( memo->some_text, "end", 3 ) == 0 )
			running = 0;
	}
	if ( shmdt( shm ) == -1)
	{ 
		perror( "shmdt" );
		exit(1);
	} 
	if( shmctl( shmid, IPC_RMID, 0 ) == -1)
	{ 
		perror( "shmctl" );
		exit(1);
	} 
	exit(0);
}

/*
kpratik@pratik-lenovo:~/Downloads/PL3$ ./client.o
Memory attached at 0x7fab5defe000
Enter 2 digit number to square:	12
*/

