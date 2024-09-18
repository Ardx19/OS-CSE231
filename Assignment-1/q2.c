#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int binarySearch(int arr[],int target,int l,int h){
    int result;
    if(l>h){
        return -1;
        // exit(0);
    }
    int mid = l+(h-l)/2;
    if(arr[mid]==target){
        return mid;
    }
    pid_t pid = fork();
    if(pid<0){
        perror("fork fail");
        exit(1);
    }
    else if(!pid){
        if(arr[mid]>target){
            return binarySearch(arr,target,l,mid-1);
        }
        else{
            return binarySearch(arr,target,mid+1,h);
        }
        exit(0);
    }
    else{
        wait(NULL);
        exit(0);
    }
    return -1;
}

int main(){
    int size=16;
    int arr[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    for(int i=0;i<size;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    int target;
    printf("Enter a target value: ");
    scanf("%d",&target);
    int result = binarySearch(arr, target, 0, size - 1);
    printf("Found at index: %d\n",result);
    return 0;
}