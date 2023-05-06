Andrew Rossman
823166078
Due 05/07/23

File Manifest
MergeSort/
    MergeSort.cpp
    MergeSortCaller.cpp
    Caller
    MergeSort

MonteCarlo/
    MonteCarlo.cpp
    MonteCarloCaller.cpp
    MonteCarlo
    Caller

readme.txt

Compilation Instructions
upcxx -O -Wall -o MergeSort MergeSort.cpp -lm
upcxx -O -Wall -o MonteCarlo MonteCarlo.cpp -lm
g++ -g -Wall MonteCarloCaller.cpp -o Caller
g++ -g -Wall MergeSortCaller.cpp -o Caller

Running Instructions
upcxx-run -n <PROCESS_COUNT> ./MergeSort <ELEMENT_COUNT>
Process Count: Amount of processes to run in MPI. If you want a large amount of processes, you must include the "-oversubscribe" option after mpiexec and before the program title, ./MergeSort.
Element Count: Integer Amount of elements inside of the array. If lower than 31, then it will use 2 to the power of the given value.

upcxx-run -n <PROCESS_COUNT> ./MonteCarlo <TOSS_COUNT>
Process Count: Amount of processes to run in MPI. 
Toss Count: amount of tosses to be made by the program in total. If this amount is less than 31, it will be squared by the program.

Alternatively, 
./Caller <TOSS_COUNT>
Toss Count: amount of tosses to be made by the program in total. If this amount is less than 31, it will be squared by the program.
You can supply multiple toss counts as long as they are comma separated

./Caller <ELEMENT_COUNT>
Element Count: Integer Amount of elements inside of the array. If lower than 31, then it will use 2 to the power of the given value.
You can supply multiple element counts as long as they are comma separated

Tables for the time of the functions
TODO MonteCarlo Times, Speedups, Efficiency
TODO MergeSort Times, Speedups, Effieciency
MonteCarlo Times
d\p     1           2           4           8           16          32   
5       1.274648    0.242665    0.010049    0.032200    0.028656    0.030952
6       1.066810    0.069386    0.011134    0.019441    0.017740    0.013491
7       0.858725    1.882448    0.011155    0.040602    0.010777    0.040864
8       0.658669    1.674436    0.010878    0.009900    0.005920    0.027161
9       0.450757    1.466522    0.009138    0.020168    0.032216    0.019337

MonteCarlo Speedups
d\p     1           2           4           8           16          32   
5       Serial      0.178571    0.002488    0.000776    0.000872    0.000808
6       Serial      0.004597	0.002515	0.001440	0.001578	0.002075
7       Serial      0.001266	0.003407	0.000936	0.003526	0.000930
8       Serial      0.268293	0.003034	0.003333	0.005574	0.001215
9       Serial      0.186957	0.004706	0.002132	0.001335	0.002224

MonteCarlo Efficiency
d\p     1           2           4           8           16          32   
5       Serial      0.089286	0.000622	0.000097	0.000055	0.000025
6       Serial      0.002298	0.000629	0.000180	0.000099	0.000065
7       Serial      0.000633	0.000852	0.000117	0.000220	0.000029
8       Serial      0.134146	0.000758	0.000417	0.000348	0.000038	
9       Serial      0.093478	0.001176	0.000267	0.000083	0.000069

Note that in order to produce a non-zero serial time, the program MUST time sorting regardless of the sorting method. Please note that mergesort is how the processes merge the subarrays while each subarray is individually sorted with bubble sort. 

MergeSort Times
d\p     1           2           4           8           16          32
5       0.000005    0.000112    0.087802    0.063911    0.011137    0.008804
6       0.000016    0.000094    0.000304    0.015163    0.016952    0.013639
7       0.000059    0.000128    0.076893    0.023375    0.013714    0.024121
8       0.000209    0.006093    0.000101    0.010416    0.044013    0.011547
9       0.000814    0.008159    0.063911    0.025921    0.033825    0.035650

MergeSort Speedups
d\p     1           2           4           8           16          32   
5       Serial      0.044643	0.000057	0.000078	0.000449	0.000568
6       Serial      0.170213	0.052632	0.001055	0.000944	0.001173
7       Serial      0.460938	0.000767	0.002524	0.004302	0.002446
8       Serial      0.034302	2.069307	0.020065	0.004749	0.018100
9       Serial      0.099767	0.012736	0.031403	0.024065	0.022833

MergeSort Efficiency
d\p     1           2           4           8           16          32   
5       Serial     0.022321	    0.000014	0.000010	0.000028	0.000018	 
6       Serial     0.085106	    0.013158	0.000132	0.000059	0.000037
7       Serial     0.230469	    0.000192	0.000316	0.000269	0.000076
8       Serial     0.017151	    0.517327	0.002508	0.000297	0.000566
9       Serial     0.049884 	0.003184	0.003925	0.001504	0.000714 


For the runs of the MonteCarlo and MergeSort algorithm, efficiency generally increase as number of inputs increase, showing that multiple threads are better able to handle larger amounts of data but do not improve time when dealing with smaller amounts of data. 
It is also noted that the speedups of both algorithms are best when the amount of threads are close to the mid point. Too many threads overburden the processor, and result in decreased speedup while a moderate amount of threads increase speedup while not creating too much overhead for the processor to work over. 
