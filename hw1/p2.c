#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    const char *name = "HW1P2";
    const int SIZE = 4096;
    
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t pid;

    pid = fork();
    if (pid < 0){
        fprintf(stderr, "Fork Failed\n");
        shm_unlink(name);
        return 1;
    }
    /*child process*/
    else if (pid == 0){
        int num = atoi(argv[1]);
        // printf("%d ", num);
        ptr += sprintf(ptr, "%d ", num);
        while (num != 1){
            if (num % 2 == 0){
                num /= 2;
                // printf("%d ", num);
                ptr += sprintf(ptr, "%d ", num);
            }
            else{
               num = 3 * num + 1;
            //    printf("%d ", num);
               ptr += sprintf(ptr, "%d ", num);
            }
        }
        // printf("\n");
        printf("Child Process completely write the content of the sequence to the share memory object\n");
    }
    /*parent process*/
    else{
        printf("Parent is waiting for child to complete...\n");
        wait(NULL);
        shm_fd = shm_open(name, O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("%s\n", (char *)ptr);
        shm_unlink(name);
        printf("Child Complete...\n");
    }
    return 0;
}