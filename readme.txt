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

MonteCarlo Times
d\p     1           2           4           8           16          32   
5       0.638700    0.941487    0.421291    1.994995    1.772299    0.363242
6       0.490884    0.831517    0.308629    1.887804    1.919942    0.672728
7       0.807456    0.708483    0.171615    1.876833    1.996312    1.006053
8       0.778739    0.538813    0.124724    1.738860    0.016062    1.244255
9       0.826243    0.344345    0.063418    1.684333    0.073928    1.620523

MonteCarlo Speedups
1.	2.	        4.      	8.      	16.     	32.
1	0.678394	1.516054	0.320151	0.360379	1.758331
1	0.590347	1.590531	0.260029	0.255676	0.729691
1	1.139697	4.705043	0.430222	0.404473	0.802597
1	1.445286	6.243698	0.447844	48.48331	0.625867
1	2.399462	13.02852	0.490546	11.17632	0.509861

MonteCarlo Efficiency
1.	2.	        4.	        8.	        16.     	32.
1.	0.339197	0.379013	0.040018	0.022523	0.054947
1.	0.295173	0.397632	0.032503	0.015979	0.022802
1.	0.569848	1.176260	0.053777	0.025279	0.025081
1.	0.722643	1.560924	0.055980	3.030207	0.019558
1.	1.199731	3.257131	0.061318	0.698520	0.015933

Note that in order to produce a non-zero serial time, the program MUST time sorting regardless of the sorting method. Please note that mergesort is how the processes merge the subarrays while each subarray is individually sorted with bubble sort. 

MergeSort Times
d\p     1           2           4           8           16          32
5       1.415681    0.596638    0.510098    1.887272    1.777057    0.200951
6       1.223535    0.504741    0.365526    1.783795    1.823875    0.306966
7       1.031607    0.407409    0.232903     1.772075   1.833086    0.569384
8       0.845196    1.033920    0.132440   1.741531    1.850889    0.776589
9       0.728178    0.545594    1.973824    1.706865    1.949022    1.022426

MergeSort Speedups
1	2	        4	        8	        16	        32
1	2.372763	2.775311	0.750120	0.796643	7.044906
1	2.424084	3.347326	0.685916	0.670843	3.985897
1	2.532116	4.429341	0.582146	0.562770	1.811794
1	0.817467	6.381727	0.485317	0.456643	1.088344
1	1.334651	0.368917	0.426617	0.373611	0.712206

MergeSort Efficiency
1.	2.	        4.	        8.      	16      	32
1.	1.186381	0.693827	0.093765	0.049790	0.220153
1.	1.212042	0.836831	0.085739	0.041927	0.124559
1.	1.266058	1.107335	0.072768	0.035173	0.056618
1.	0.408733	1.595431	0.060664	0.028540	0.034010
1.	0.667325	0.092229	0.053327	0.023350	0.022256


There were several inconistent wall times across the calls. Mergesort [2,8] has a full second when all other calls in that column has less. This may be a result of the use of the gettimeofday() function giving incorrect times for the total time of the function. 
