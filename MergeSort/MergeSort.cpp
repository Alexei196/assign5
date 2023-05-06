#include<stdio.h>
#include<sys/time.h>
#include<upcxx/upcxx.hpp>
#include<vector>
#include<math.h>

#define TIME(timeStruct) (double) timeStruct.tv_sec + (double) timeStruct.tv_usec * 0.000001
#define MAX_RAND_INT 10000

void mergeArray(std::vector<int>);
std::vector<int> sort(std::vector<int>);
//I picked a vector in place of an array so that the merge sort algorithm can be trivially solved with fire and forget RPC
//Globally defined so that remote procedure can access array without using a parameter 
std::vector<int> myVector;
//argument 1 is the ending array size of the array
int main(int argc, char** argv) {
    upcxx::init();
    int my_rank = (int) upcxx::rank_me();
    int arrayTotalSize;
    bool printArrays = false;
    //Init checking by the root process
    if(upcxx::rank_me() == 0) {
        if(argc < 2) {
            fprintf(stderr, "Not enough arguments!\n");
            upcxx::finalize();
            return 1;
        } 
        arrayTotalSize = atoi(argv[1]);
        if(arrayTotalSize < 1) {
            fprintf(stderr, "Array size cannt be less than 1!");
            upcxx::finalize();
            return 1;
        }
        if((log2(upcxx::rank_n()) - floor(log2(upcxx::rank_n()))) != 0) {
            fprintf(stderr, "Process amount must be a base of 2!\n");
            upcxx::finalize();
            return 1;
        }
        if(arrayTotalSize < 31) {
            arrayTotalSize = (int)pow(2, arrayTotalSize);
        }
    }
    //Bcast array size to all processes
    arrayTotalSize = upcxx::broadcast(arrayTotalSize, 0).wait();
    //init variables used in process
    timeval startTime, endTime;
    //Parallel Time weakness solved at seed level by multiplying by rank
    srand(time(NULL) * (1+upcxx::rank_me()));
    int myVectorSize = arrayTotalSize / upcxx::rank_n();
    int p = (int) log2((double) upcxx::rank_n());
    //Generate vector
    myVector = std::vector<int>();
    for(int i =0; i < myVectorSize; ++i) {
        myVector.emplace_back(rand() % MAX_RAND_INT);
    }
    //print init vector
    if(printArrays) {
        printf("Rank %d: [", upcxx::rank_me());
        for(int i = 0; i < myVector.size(); ++i) {
            printf("%d, ", myVector[i]);
        }
        printf("] with size %ld\n", myVector.size());
    }
    gettimeofday(&startTime, NULL);
    upcxx::barrier();
    //bubble sort init arrays under the watch of the timer
    myVector = sort(myVector);
    //modified borrowed merge sort algorithm
    for(int level = 0; level < p; ++level) {
        //Only conerned ranks should transmit their vectors
        //concerned ranks must transmit their data to the target process 
        if(my_rank % ((int)pow(2, level+1)) != 0 && my_rank % ((int)pow(2, level)) == 0) {
            upcxx::rpc_ff(my_rank - (int)pow(2, level), mergeArray, myVector);
        }
        //process fires its held vector to target process and moves on
        upcxx::barrier();
    }
    //End of Mergesort
    //get and reduce time
    gettimeofday(&endTime, NULL);
    double totalTime = TIME(endTime) - TIME(startTime);
    totalTime = upcxx::reduce_one(totalTime, upcxx::op_fast_max, 0).wait();
    if(upcxx::rank_me() == 0) {
        if(printArrays) {
            printf("End Array : ");
            for(int i =0; i < myVector.size(); ++i) {
                printf("%d, ", myVector[i]);
            }
            printf("\nVector size %ld\n", myVector.size());
        } else {
            printf("Process complete, ");
        }
        printf("My time is %f\n", totalTime);
    }
    //finish
    upcxx::finalize();
    return 0;   
}
//RPC function to merge sent over and locally-global array
void mergeArray(std::vector<int> otherVector) {
    std::vector<int> newVector = std::vector<int>();
    int m = 0, o = 0;
    //Iterate and merge separate vector
    while(m < myVector.size() && o < otherVector.size() ) {
        if(otherVector[o] > myVector[m]) {
            newVector.emplace_back(myVector[m++]);
        } else {
            newVector.emplace_back(otherVector[o++]);
        }
    }
    //Append leftover elements of either vectors
    while(m < myVector.size()) {
        newVector.emplace_back(myVector[m++]);
    }

    while(o < otherVector.size()) {
        newVector.emplace_back(otherVector[o++]);
    }
    //reassign old vector to new
    myVector.assign(newVector.begin(), newVector.end());
}
//helper sort method
std::vector<int> sort(std::vector<int> array) {
    int swapped;
    do {
        swapped = 0;
        for(int i = 1; i < array.size(); ++i) {
            if(array[i-1] > array[i]) {
                //swap
                int temp = array[i-1];
                array[i-1] = array[i];
                array[i] = temp;
                swapped = 1;
            }
        }
    } while(swapped);
    return array;
}