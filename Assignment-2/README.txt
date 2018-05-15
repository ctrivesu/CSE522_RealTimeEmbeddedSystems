NAME: Sushant Trivedi
ASU ID: 1213366971
Arizona State University


---------------
FILE SOURCES
---------------
->  C programs: Code-Final.c
->  Inputfile: Specification.txt

---------------
INSTRUCTIONS TO COMPILE AND RUN
---------------
1. Run the Make file to compile and run ./CP.o
2. In order to run the Specification, please use Specification.txt as source. You can modify the text file on Line 538. 
3. Once running the code, Make choice for showing comments or not
4. Next, Select 1 from menu for File run task specs and 2 for sythetic task generation.
4. For file run spec, In each Taskset,it denotes start of task and if RT test is done, and the response time and whether it is Schedulable or Not.
5. For synthetic tasksets, it stores data into Following Files. This naming convention is generalized as per the no of tasks per taskset. Can be edited on line 728.
TASKSET 10 0 -> 10 tasks per taskset, deadline [Ci,Ti]
TASKSET 25 0 -> 25 tasks per taskset, deadline [Ci,Ti]
TASKSET 10 1 -> 10 tasks per taskset, deadline [Ci+(Ci-Ti)/2, T]
TASKSET 10 1 -> 25 tasks per taskset, deadline	[Ci+(Ci-Ti)/2, T]


---------------
BASIC ERRORS
---------------
1. While Running RT Test it may get stuck at the convergence. Thus, rerun the execution. Though haven't faced it in this code yet.
2. The no of Taskset needs to be modified at the start of the file under the variable “max_taskset.

---------------
SYSTEM REQUIREMENTS
---------------
SDK: GCC
GCC: Minimum version of 4.8 is required to run -pthread option while compiling.



