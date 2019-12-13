/* Parameters:
 * number of producers
 * number of items each producer should produce
 * number of consumers
 * number of items each consumer should consume
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PRODUCER_BIN "./producer"
#define CONSUMER_BIN "./consumer"

#define BLOCK_SIZE 32

int main(int argc, char **argv){
    int producer_count = atoi(argv[1]);
    int producer_item = atoi(argv[2]);
    int consumer_count = atoi(argv[3]);
    int consumer_item = atoi(argv[4]);
    printf("producer count: %d\n", producer_count);
    printf("producer items: %d\n", producer_item);
    printf("consumer count: %d\n", consumer_count);
    printf("consumer items: %d\n", consumer_item);
    
    pid_t *pids;
    pids = (pid_t *)malloc(sizeof(pid_t) * (producer_count + consumer_count));

    for(int i = 0; i < producer_count; i++){
        if((pids[i] = fork()) == 0){
            //char **arguments = (char **)malloc(sizeof(char *) * 3);
            char index_buffer[128] = {0};
            snprintf(index_buffer, sizeof(index_buffer), "%d", i);
            char *arguments[4] = {PRODUCER_BIN, index_buffer, argv[2], NULL};
            execvp(PRODUCER_BIN, arguments);
        }
    }

    for(int i = 0; i < consumer_count; i++){
        if((pids[i + producer_count] = fork()) == 0){
            char index_buffer[128] = {0};
            snprintf(index_buffer, sizeof(index_buffer), "%d", i);
            char *arguments[4] = {CONSUMER_BIN, index_buffer, argv[4], NULL};
            execvp(CONSUMER_BIN, arguments);
        }
    }
    
    int status;
    for(int i = 0; i < (consumer_count + producer_count); i++){
        waitpid(pids[i], &status, 0);
    }

    printf("all the processes have been terminated\n");
    /*
    while(1);
    int fd = open("/dev/scullbuffer", O_RDWR);
    printf("the file descriptor is %d\n", fd);
    sleep(2);
    char haha[32];
    int result = 50;


    printf("start write\n");
    for(int i = 0; i <= 100; i++){
        haha[0] = i+100;
        result = write(fd, haha, 32);
        printf("the first character is %d\n", haha[0]);
        printf("write result is %d\n", result);
    }

    char read_array[32];
    printf("start read\n");
    for(int i = 0; i <= 50; i++){
        result = read(fd, read_array, 32);
        printf("the first character in read is %d\n", read_array[0]);
        printf("read result is %d\n", result);
    }

        
    
    close(fd);
    return 0;
    */
}
