#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "producer.h"

int producer_fd = 0;

void producer_init()
{
	producer_fd = open("/dev/scullBuffer0", O_WRONLY);
	if ( producer_fd < 0 ) {
		printf("producer_init: error opening file: %s\n", strerror(errno));
		return;	
	}
}

void producer_write()
{
}

void producer_close()
{
	close(producer_fd);
}

