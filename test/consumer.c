/* Parameters:
 * index of this consumer
 * items this consume need to consume
 */
#include "./shared.h"

char data[32];
char log_file_name[1024];

int main(int argc, char **argv){
    int consumer_index = atoi(argv[1]);
    int consumer_item = atoi(argv[2]);

    printf("this is consumer %d, I will consume %d items\n", consumer_index, \
                consumer_item);
    int fd = open(DEVICE_NAME, O_RDONLY);
    snprintf(log_file_name, sizeof(log_file_name), "consumer%d.log", consumer_index);
    int log_fd = open(log_file_name, O_WRONLY|O_CREAT|O_TRUNC);
    dup2(log_fd, 1);

    sleep(2);
    for(int i = 0; i < consumer_item; i++){
        memset(data, 0, BLOCK_SIZE);
        int result = read(fd, data, BLOCK_SIZE);
        if(result == 0){
            //printf("all producers exited, I have nothing to consume\n");
            exit(0);
        }else{
        //printf("read %d bytes from device, produce by producer: %d, item index: %d\n", result, data[0], data[1]);
            printf("%d, %d, %d\n", data[0], data[1], result);
        }
    }

    return 0;
}
