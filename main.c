#include <stdio.h>

#include "consumer.h"
#include "producer.h"

int main()
{
	int prod_size, cons_size;

	consumer_init();
	producer_init();

	prod_size = producer_write();
	printf("Wrote %d bytes to scullbuffer\n", prod_size);

	cons_size = consumer_read();
	printf("Read %d bytes from scullbuffer\n", cons_size);
	
	consumer_close();
	producer_close();
	return 0;
}
