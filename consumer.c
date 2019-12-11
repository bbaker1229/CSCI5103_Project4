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

int consumer_read()
{
	char buf[512];
	int retval;

	retval = read(consumer_fd, buf, strlen(buf));

	printf("%s\n", buf);

	return retval;	
}

void consumer_close()
{
	close(consumer_fd);
}
