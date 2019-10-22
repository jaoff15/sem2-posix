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
#include <iostream>
#include <mqueue.h>

/* Thread start function: display address near top of our stack,
 and return upper-cased copy of argv_string */

int counter;

#define MESSAGE_QUEUE  "/message_queue"
#define MAX_SIZE    1024
mqd_t message_queue;
#define PMODE 0655

void* producer(void *ptr) {
	int i;
	mqd_t mq = mq_open(MESSAGE_QUEUE, O_WRONLY);
	for (i = 0; i < 500; i++) {
		counter++;
		std::string s = std::to_string(counter);
		mq_send(mq, s.c_str(), 3, 0);
	}
}

void* consumer(void *ptr) {
	mqd_t mq = mq_open(MESSAGE_QUEUE, O_RDONLY);
	char buffer[MAX_SIZE + 1];
	int i;
	for (i = 0; i < 500; i++) {
		mq_receive(mq, buffer, MAX_SIZE, NULL);
		printf("%s, ", buffer);
	}

}

int main() {
	pthread_t thread1, thread2;
	int iret1, iret2;

	counter = 0;

	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 3;
	message_queue = mq_open(MESSAGE_QUEUE, O_CREAT, PMODE, &attr);

	/* Create independent threads each of which will execute function */
	iret1 = pthread_create(&thread1, NULL, producer, NULL);
	iret2 = pthread_create(&thread2, NULL, consumer, NULL);

	/* Wait till threads are complete before main continues. Unless we  */
	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Thread 1 returns: %d\n", iret1);
	printf("Thread 2 returns: %d\n", iret2);
	exit(0);
}

