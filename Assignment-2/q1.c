/*
Perform parallel merge sort on a 16-element array. The 16-element array would be
divided into two 8-element arrays, and each one would be passed to a child process.
Similarly, the children would pass 4-element arrays to their children and so on. Each
process will wait for its children and then merge the resulting arrays using a merge
operation. Use pipes to pass data among processes. [10 pts]
Input: Define a 16-element array in the code. The array must be unsorted.
Output: Print the initial unsorted arslray and the final sorted array.
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<errno.h>

void mergeSort(int arr[],int n){

}

int main(){
    int arr[] = {12,4,0,4,2,7,1,-3,-7,2,7,12,67,5,0,-11};
    printf("%d\n",getppid());
}
