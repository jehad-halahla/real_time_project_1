#include "includes/include.h"

int main() {

    //we will fork a child and execute the child program
    int pid = fork();

    if (pid == 0) {
        execlp("./child", (char*) NULL);
    } else {
        printf("Parent process with PID %d\n", getpid());
    }
    return 0;
}