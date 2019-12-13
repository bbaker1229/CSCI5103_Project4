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

int producer_write()
{
	char buf[512] = {0,};
	int retval;

	FILE *f;

	f = fopen("readthis.txt", "r");

	fscanf(f, "%[^\n]", buf);
	retval = write(producer_fd, buf, sizeof(buf));

//	printf("Enter some text:\n");
//	fgets(buf, sizeof(buf), stdin);

//	retval = write(producer_fd, buf, strlen(buf));

	fclose(f);

	return retval;
}

void producer_close()
{
	close(producer_fd);
}

