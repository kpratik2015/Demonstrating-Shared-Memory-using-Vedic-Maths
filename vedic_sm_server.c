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
	int running=1;
	int *shm = NULL;
	struct shared_stuff_st *memo;
	int num, near, deviation, ls, rs, temp, shmid, numSq;
	char finalSq[20], cls[20], crs[20];
	key_t key;
	key = 1234;	//we'll name our shared memory segment 1234
	
	//1024 is segment size. 3rd arguement is permission. You can write only 0666 too (read, write, execute).
	
	//CREATE SEGMENT
	if ( ( shmid = shmget( key, 1024, IPC_CREAT | 0666) ) == -1 )
	{ 
		perror("shmget");
		exit(1);
	}
	
	//ATTACH SEGEMENT TO OUR DATA SPACE
	//void *shmat(int shmid, void *shmaddr, int shmflg);
	//shmflg can be set to SHM_RDONLY if you only want to read from it, 0 otherwise.
	if ( ( shm = shmat(shmid, NULL, 0 ) ) == (int*) -1)
	{ 
		perror("shmat");
		exit(1);
	}
	
	printf( "Memory attached at %p\n", (int*)shm );	//%x means that printf will output its value in hexadecimal format. And %p is for pointer
	memo = (struct shared_stuff_st*) shm;
	
	
	while(running)
	{ 
		if ( memo -> written_by_you )
		{ 
			
			printf("Finding square...\n");
			
			num = memo -> numToSq;		//num = 12
			
			near = (int) num/10;		//near = 1
			near = near * 10;			//near = 10
			deviation = num - near;		//deviaiton = 2
			
			ls = num + deviation;		//ls = 14
			temp = (int) num/10;		//temp = 1
			ls = ls * temp;				//ls = 14	****FINAL****
			if(num < 100 && ls > 100)	//12 < 100 && 14 > 100
				ls = (int) ls/10;
				
			rs = deviation * deviation;	//rs = 4	****FINAL****
			if(rs > 10){				//4 > 10
				temp = rs%10;
				ls = temp + ls;
			}
			//The C library function int sprintf(char *str, const char *format, ...) sends formatted output to a string pointed to, by str.
			sprintf(cls ,"%d" , ls);	
			sprintf(crs ,"%d" , rs); 
			printf("Square is: \t");	
			printf("%s \n", strcat(cls, crs));	//144
			sleep(1);
			printf( "This is the %s \n", memo->some_text );
			sleep(1);	
			
			memo -> written_by_you = 0;
			if ( strncmp( memo -> some_text, "end", 3)== 0)
				running = 0;
		}
	} 
	if ( shmdt( shm ) == -1 )
	{ 
		perror( "shmdt" );
		exit(1);
	}
		exit(0);
}

/*
kpratik@pratik-lenovo:~/Downloads/PL3$ ./server.o
Memory attached at 0x7fba5e2f5000
Finding square...
Square is: 	144 
This is the end 
*/
