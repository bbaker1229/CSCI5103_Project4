#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "consumer.h"

int consumer_fd = 0;

void consumer_init()
{
	consumer_fd = open("/dev/scullBuffer0", O_RDONLY);
	if ( consumer_fd < 0 ) {
		printf("consumer_init: error opening file: %s\n", strerror(errno));
		return;	
	}

}

void consumer_read()
{
}

void consumer_close()
{
	close(consumer_fd);
}
