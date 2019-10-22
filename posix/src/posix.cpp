//============================================================================
// Name        : posix.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <iostream>


/* Thread start function: display address near top of our stack,
   and return upper-cased copy of argv_string */

int counter;
pthread_mutex_t lock;

void *print_number(void *ptr){
	int i;
	pthread_mutex_lock(&lock);
	counter = 0;
	for(i=0;i < 500; i++){
//		pthread_mutex_lock(&mutex_counter);
		counter++;
		printf("%d, ",counter);
//		pthread_mutex_unlock(&mutex_counter);
	}
	pthread_mutex_unlock(&lock);
}

int main()
{
     pthread_t thread1, thread2;
     int  iret1, iret2;

     pthread_mutex_init(&lock, NULL);

    /* Create independent threads each of which will execute function */
     iret1 = pthread_create( &thread1, NULL, print_number, &counter);
     iret2 = pthread_create( &thread2, NULL, print_number, &counter);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL);

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     exit(0);
}




