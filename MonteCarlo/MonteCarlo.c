#include<stdio.h>
#include<stdlib.h>

#define RAND_D ( -1.0 + ( (double)rand() / (RAND_MAX / 2.0)))

long long int makeTosses(long long int);

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "ERROR: Not enough arguments\n");
        //TODO abort upcxx
        return 1;
    }
    long long int totalTosses = strtoll(argv[1]);
    if(totalTosses < 1) {
        fprintf(stderr, "invalid count of tosses\n");
        //TODO abort upcxx
        return 1;
    }
    //TODO use upcxx to assign threadCount and rank
    int threadCount, my_rank;
    double wTime = 0;

    //TODO access need to broadcast elements per thread
    long long int assignedTosses = totalTosses / threadCount;
    long long int remainderTosses = totalTosses - (assignedTosses * threadCount);
    //TODO share landedTosses
    long long int landedTosses = 0;
    if(my_rank < remainderTosses) {
        ++assignedTosses;
    }
    landedTosses += makeTosses(assignedTosses);

    //TODO Reduction on wTime
    double wTime = 1;
    if(my_rank == 0) {
        double piEstimate = 4.0l * (landedTosses / (double) totalTosses);
        printf("I estimate pi to be %d\n", piEstimate);
        printf("My wall time is %f\n", wTime);
    }
    return 0;
}

long long int makeTosses(long long int assignedTosses) {
    long long int landedSum;
    for(int toss = 0; toss < assignedTosses; ++i) {
        double x = RAND_D;
        double y = RAND_D;
        if((x*x + y*y) < 1.0) {
            landedSum+=1ll;
        }
    }
    //TODO confirm reduction of sums
    return landedSum;
}