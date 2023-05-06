#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv) {
    int processCount[] = {1,2,4,8,16,32}, errorCode, processesSpecified = 6, error = 0;
    char commandBuffer[128];

    int* passedIntegers = (int*) malloc(argc * sizeof(int));
    for(int i = 1; i < argc; ++i) {
        if(atoi(argv[i]) <= 0){
            fprintf(stderr, "Cannot use \"%s\"!\n", argv[i]);
            continue;
        }
        passedIntegers[i-1] = atoi(argv[i]);
    }

    //for each value input
    for(int pIndex = 0; pIndex < processesSpecified; ++pIndex) {
        for(int tIndex = 0; tIndex < argc-1; ++tIndex) {
            sprintf(commandBuffer, "~/upcxx/bin/upcxx-run -localhost -n %d ./MonteCarlo %d", processCount[pIndex], passedIntegers[tIndex]);
            printf("\nRunning p=%d, d=%d:\n", processCount[pIndex], passedIntegers[tIndex]);
            errorCode = system(commandBuffer);
            if(errorCode) {
                fprintf(stderr, "p=%d, d=%d call failed with error: %d\n", processCount[pIndex], passedIntegers[tIndex], errorCode);
                error += 1;
            }
        }   
    }
    printf("I report %d errors.\n", error);
    free(passedIntegers);
    return 0;   
}