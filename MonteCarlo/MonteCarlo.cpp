#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<upcxx/upcxx.hpp>

#define RAND_D ( -1.0 + ( (double)rand() / (RAND_MAX / 2.0)))
#define TIME(timeStruct) (double) timeStruct.tv_sec + (double) timeStruct.tv_usec * 0.000001

long long int makeTosses(long long int);

//One argument: toss count in form of a number
int main(int argc, char** argv) {
    upcxx::init();
    srand(time(NULL) * (1+upcxx::rank_me()));
    timeval startTime;
    timeval endTime;
    long long int totalTosses;

    if(upcxx::rank_me() == 0) {
        if(argc < 2) {
            fprintf(stderr, "ERROR: Not enough arguments\n");
            return 1;
        }
        totalTosses = strtoll(argv[1], NULL, 10);
        if(totalTosses < 1) {
            fprintf(stderr, "invalid count of tosses\n");
            return 1;
        }
    }
    gettimeofday(&startTime, NULL);
    totalTosses = upcxx::broadcast(totalTosses, 0).wait();

    //TODO access need to broadcast elements per thread
    long long int assignedTosses = totalTosses / upcxx::rank_n();
    long long int remainderTosses = totalTosses - (assignedTosses * upcxx::rank_n());

    long long int processTosses = 0;

 
    if(upcxx::rank_me() < remainderTosses) {
        processTosses = makeTosses(++assignedTosses);
    } else {
        processTosses = makeTosses(assignedTosses);
    }
    
    long long int landedTosses = upcxx::reduce_all(processTosses, upcxx::op_fast_add).wait();
    
    gettimeofday(&endTime, NULL);
    double wTime = TIME(endTime) - TIME(startTime);
    wTime = upcxx::reduce_all(wTime, upcxx::op_fast_max).wait();
    if(upcxx::rank_me() == 0) {
        double piEstimate = 4.0l * (landedTosses / (double) totalTosses);
        printf("I estimate pi to be %f\n", piEstimate);
        printf("My wall time is %f\n", wTime);
    }
    upcxx::finalize();
    return 0;
}

long long int makeTosses(long long int assignedTosses) {
    long long int landedSum;
    for(int toss = 0; toss < assignedTosses; ++toss) {
        double x = RAND_D;
        double y = RAND_D;
        if((x*x + y*y) < 1.0) {
            landedSum+=1ll;
        }
    }
    
    return landedSum;
}