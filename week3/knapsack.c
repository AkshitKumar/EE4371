#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int count = 0; // Counter to accumulate the number of times knapsack is called
int n;
/*
    Recursive Pseudocode implementation of knapsack
    @param target : Target interger
    @param candidate : Element of the array weights
    @param weights : POsitive integer weights w1,w2,w3,...
*/
bool knapsack(int target, int candidate, int weights[]){
    count++;
    if(target == 0){
        return true;
    }
    else if(target < 0 || candidate > n){
        return false;
    }
    else if(knapsack(target - weights[candidate],candidate+1,weights)){
        return true;
    }
    else{
        return knapsack(target,candidate+1,weights);
    }
}

int main(){
    int candidate;
    int min[20],max[20];
    int target;
    for(int i = 0; i < 20; i++){
        min[i] = 0;
        max[i] = 0;
    }
    for(int i = 0; i< 10000; i++){
        n = (int)(rand()%20) + 1; // n uniformly distrbuted between 0 and 19
        target = round(rand()%(int)((pow(n,2)/2)+1)); // uniformly random from 0 to n*n/2
        int weights[n];
        for(int p = 0; p < n; p++){
            weights[p] = round(rand()%n); // uniformly random weights from 0 to n
        }
        knapsack(target,0,weights);
        // Populating the table Elements
        if(min[n-1] == 0 || min[n-1] > count){
            min[n-1] = count;
        }
        if(max[n-1] == 0 || max[n-1] < count){
            max[n-1] = count;
        }
        // Reset the count to 0
        count = 0;
    }
    // Printing the table
    printf("N\tMin\tMax\n");
    for(int i = 0; i < 20; i++){
        printf("%d\t%d\t%d\n",i+1,min[i],max[i]);
    }
}


/* Program Complexity
N	Min	Max
1	1	1
2	1	15
3	1	31
4	1	63
5	1	127
6	1	255
7	1	511
8	1	1023
9	1	2047
10	1	4095
11	1	8191
12	1	16383
13	1	32767
14	1	65535
15	1	131071
16	1	262143
17	1	524287
18	1	1048575
19	1	2097151
20	1	4194303
Maximum is 2^(N+2)-1. This occurs when no combination of weights can give a total of target and an exhaustive search of all possibilities is required.
Minimum is 1 wherein zero weight can be carried in the knapsack.
*/
