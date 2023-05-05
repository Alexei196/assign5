#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<upcxx/upcxx.hpp>
#include<math.h>

int main(int argc, char** argv) {
    upcxx::init();
    int arraySize = 0;
    if(upcxx::rankme() == 0) {
        if(argc < 2) {
            fprintf(stderr, "Not enough arguments!\n");
            return 1;
        } 
        arraySize = atoi(argv[1]);
        if(arraySize < 1) {
            fprintf(stderr, "Count cannt be less than 1");
        }
    }
    timeval startTime, endTime;
    srand(time(NULL) * (1+upcxx::rank_me()));
    int arraySize = upcxx::broadcast(arraySize, 0).wait();
    int sizePerProcess = arraySize / upcxx::rank_n();

    int myArray[sizePerProcess];
    for(int i =0; i < sizePerProcess; ++i) {
        myArray[sizePerProcess] = rand();
    }

    for(int level = 0; level <= p; ++level) {
        for(int idx = 0; idx < 2^p; idx+=2^level) {
            int dest;
            if(mod(idx, 2^(level+1)) == 0) {
                dest = idx;
            } else {
                dest = idx - 2^level;
            }

            //process idx spends array to process dest
            if(upcxx::rank_me() == idx) {
                upcxx::rpc(dest, mergeArray, myArray).wait();
            }
        }
    }


    upcxx::finalize();
    return 0;   
}
