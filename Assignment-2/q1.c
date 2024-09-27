#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<errno.h>

void merge(int arr[],int l,int mid, int h){
    int n1=mid-l+1;
    int n2=h-mid;

    int left[n1],right[n2];

    for(int i=0;i<n1;i++){
        left[i]=arr[l+i];
    }
    for(int j=0;j<n2;j++){
        right[j]=arr[mid+j+1];
    }

    int i=0;
    int j=0;
    while(i<n1 && j<n2){
        if(left[i]<=right[j]){
            arr[l]=left[i];
            i++;
        }
        else{
            arr[l]=right[j];
            j++;
        }
        l++;
    }

    while(i<n1){
        arr[l]=left[i];
        i++;
        l++;
    }
    while(j<n2){
        arr[l]=right[j];
        j++;
        l++;
    }
}

void mergeSort(int arr[],int l,int h){
    if(l<h){
        int mid = l+(h-l)/2;
        int pipe1[2],pipe2[2];
        if(pipe(pipe1) == -1|| pipe(pipe2)==-1 ){ printf("pipe could not be created"); exit(1);}

        int id=fork();
        if(id==-1){exit(0);}
        if(id==0){
            close(pipe1[0]);
            mergeSort(arr,l,mid);
            if(write(pipe1[1],arr+l,(mid-l+1)*sizeof(int))){exit(3);}
            close(pipe1[1]);
            exit(0);
        }

        int id0 = fork();
        if(id0==-1){exit(0);}
        if(id0==0){
            close(pipe2[0]);
            mergeSort(arr,mid+1,h);
            if(write(pipe2[1],arr+mid+1,(h-mid)*sizeof(int))){exit(3);}
            close(pipe2[1]);
            exit(0);
        }
        //wait(NULL) == -1 iff there are no childeren to wait for
        //wait() starts exec if any child finishes exec
        while(wait(NULL)!=-1 || errno != ECHILD ){}

        close(pipe1[1]);
        close(pipe2[1]);
        read(pipe1[0],arr+l,(mid-l+1)*sizeof(int));
        read(pipe2[0],arr+mid+1,(h-mid)*sizeof(int));
        close(pipe1[0]);
        close(pipe2[0]);

        merge(arr,l,mid,h);

    }
}

void print(int arr[],int n){
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

int main(int argc, char **argv){
    int arr[16] = {12,4,0,4,2,7,1,-3,-7,2,7,12,67,5,0,-11};

    printf("Initial Array:");
    print(arr,16);

    mergeSort(arr,0,15);

    printf("Final Array:");
    print(arr,16);
}

//!!!THIS SHIT WORKS DONT TOUCH!!!