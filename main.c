#include <stdio.h>
#include <pthread.h>

#include "consumer.h"
#include "producer.h"

//pthread_t consumer_handle;
//pthread_t producer_handle;

void *consumer_thread(void* argument)
{
	//int cons_size;
	consumer_init();

	while( consumer_read() > 0) {
		//printf("Read %d bytes from scullbuffer\n", cons_size);
	}
	
	consumer_close();
	return NULL;
}

void *producer_thread(void* argument)
{
	int prod_size;
	producer_init();

	for (int i = 0; i < 10; i++) {
		prod_size = producer_write();
		printf("Wrote %d bytes to scullbuffer\n", prod_size);
	}

	producer_close();
	return NULL;
}

int main()
{
	int i;

	pthread_t consumer_handle;
	pthread_t producer_handle;
	
	pthread_create(&producer_handle, NULL, producer_thread, NULL);
	for (i = 0; i < 3; i++) {
		pthread_create(&consumer_handle, NULL, consumer_thread, NULL);
	}

	//pthread_join(consumer_handle, NULL);
	//pthread_join(producer_handle, NULL);

	pthread_exit(NULL);

	printf("consumers and producers finished!\n");

	return 0;
}
