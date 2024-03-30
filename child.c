#include "includes/include.h"

int main(int argc, char* argv[]) {

    // TODO: if argc != numberOfArgs.. 

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    
    }

        printf("Child process with PID %d\n", getpid());

    return 0;
}
