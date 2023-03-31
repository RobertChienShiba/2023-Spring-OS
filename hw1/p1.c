#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    pid_t pid;

    pid = fork();
    if (pid < 0){
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
     /*child process*/
    else if(pid == 0){
        int num = atoi(argv[1]);
        printf("%d ", num);
        while (num != 1){
            if (num % 2 == 0){
                num /= 2;
                printf("%d ", num);
            }
            else{
               num = 3 * num + 1;
               printf("%d ", num);
            }
        }
        printf("\n");
    }
    /*parent process*/
    else{
        printf("Parent is waiting for child to complete...\n");
        wait(NULL);
        printf("Child Complete...\n");
    }
    return 0;
}
