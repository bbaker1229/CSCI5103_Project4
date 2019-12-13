/* Parameters:
 * index of this producer
 * items this producer need to produce
 */
#include "shared.h"
char data[32];
char log_file_name[1024];
int main(int argc, char **argv){
    int producer_index = atoi(argv[1]);
    int producer_item = atoi(argv[2]);

    printf("this is producer %d, I will produce %d items\n", producer_index, \
                producer_item);
    int fd = open(DEVICE_NAME, O_WRONLY);
    snprintf(log_file_name, sizeof(log_file_name), "producer%d.log", producer_index);
    int log_fd = open(log_file_name, O_WRONLY|O_CREAT|O_TRUNC);
    dup2(log_fd, 1);
    sleep(2);
    for(int i = 0; i < producer_item; i++){
        memset(data, 0, BLOCK_SIZE);
        data[0] = producer_index;
        data[1] = i;
        int result = write(fd, data, BLOCK_SIZE);
        if(result == 0){
            //printf("all consumers exited and there is no space, I will stop producing\n");
            exit(0);
        }else{
            printf("%d, %d, %d\n", producer_index, i, result);
        } 
    }
    return 0;
}
