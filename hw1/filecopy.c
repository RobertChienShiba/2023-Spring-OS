#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>

#define size 128

int main(int argc, char* argv[]) {
    char buffer[size];
    FILE *input = fopen(argv[1], "r");
    FILE *copy = fopen(argv[2], "w");

    int file[2];
    if (pipe(file) == -1){
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    pid_t pid;
    pid = fork();
    if (pid < 0){
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    /*child process*/
    if (pid == 0) {
        close(file[1]);
        while (read(file[0], buffer, sizeof(buffer)) > 0) {
            fputs(buffer, copy);
        }
        close(file[0]);
        fclose(copy);
    }
    /*parent process*/
    else {
        close(file[0]);
        while (fgets(buffer, size, input) != NULL) {
            write(file[1], buffer, sizeof(buffer));
            memset(buffer, 0, sizeof(buffer));
        }
        close(file[1]);
        fclose(input);
        wait(NULL);
    }
}