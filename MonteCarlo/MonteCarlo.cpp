#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<upcxx/upcxx.hpp>
#include<cmath>

#define RAND_D ( -1.0 + ( (double)rand() / (RAND_MAX / 2.0)))
#define TIME(timeStruct) (double) timeStruct.tv_sec + (double) timeStruct.tv_usec * 0.000001

long long int makeTosses(long long int);

//One argument: toss count in form of a number
int main(int argc, char** argv) {
    upcxx::init();
    srand(time(NULL) * (1+upcxx::rank_me()));
    timeval startTime, endTime;
    long long int totalTosses;
    //root checks arguments and takes input
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
        if(totalTosses < 31) {
            totalTosses = pow(2, totalTosses);
        }
    }
    gettimeofday(&startTime, NULL);
    //broadcast total amount of tosses to all processes
    totalTosses = upcxx::broadcast(totalTosses, 0).wait();

    //Broadcast tosses to all processes and do math locally
    long long int assignedTosses = totalTosses / upcxx::rank_n();
    long long int remainderTosses = totalTosses - (assignedTosses * upcxx::rank_n());
    long long int processTosses = 0;
    //Remainder handler, so that all tosses are accounted for
    if(upcxx::rank_me() < remainderTosses) {
        processTosses = makeTosses(++assignedTosses);
    } else {
        processTosses = makeTosses(assignedTosses);
    }
    //collect tosses
    long long int landedTosses = upcxx::reduce_one(processTosses, upcxx::op_fast_add, 0).wait();
    gettimeofday(&endTime, NULL);
    double totalTime = TIME(endTime) - TIME(startTime);
    totalTime = upcxx::reduce_one(totalTime, upcxx::op_fast_max, 0).wait();
    if(upcxx::rank_me() == 0) {
        double piEstimate = 4.0l * ((double)landedTosses / (double) totalTosses);
        printf("I estimated pi to be %f in %f seconds!\n", piEstimate, totalTime);
    }
    upcxx::finalize();
    return 0;
}
//helper Tosses method each process calls
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