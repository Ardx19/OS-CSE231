#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include<stdlib.h>

struct Process {
    char pid[100];
    int arrival;
    int burst;
    int remaining_burst; 
    int start_time;
    int ct; 
    int tat; 
    int rt; 
};

void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

int Partition(struct Process arr[], int l, int h, char str) {
    if (str == 'A') {
        int i = l;
        int j = h;
        int p = l;
        while (i < j) {
            while (arr[i].arrival < arr[p].arrival) {
                i++;
            }
            while (arr[j].arrival > arr[p].arrival) {
                j--;
            }
            swap(&arr[i], &arr[j]);
        }
        swap(&arr[p], &arr[j]);
        return j;
    } 
    else {
        int i = l;
        int j = h;
        int p = l;
        while (i < j) {
            while (arr[i].burst < arr[p].burst) {
                i++;
            }
            while (arr[j].burst > arr[p].burst) {
                j--;
            }
            swap(&arr[i], &arr[j]);
        }
        swap(&arr[p], &arr[j]);
        return j;
    }
}

void quickSort(struct Process arr[], int l, int h, char str) {
    if (str == 'A') {
        if (l < h) {
            int j = Partition(arr, l, h, 'A');
            quickSort(arr, l, j - 1, 'A');
            quickSort(arr, j + 1, h, 'A');
        }
    } 
    else {
        if (l < h) {
            int j = Partition(arr, l, h, 'B');
            quickSort(arr, l, j - 1, 'B');
            quickSort(arr, j + 1, h, 'B');
        }
    }
}

void copyProcesses(struct Process src[], struct Process dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// void createProcess(struct Process arr[], int n) {
//     printf("Enter details for each process:\n");
//     for (int i = 0; i < n; i++) {
//         printf("Process ID: ");
//         scanf("%s", arr[i].pid);

//         printf("Arrival Time: ");
//         scanf("%d", &arr[i].arrival);

//         printf("Burst Time: ");
//         scanf("%d", &arr[i].burst);

//         arr[i].remaining_burst = arr[i].burst;
//         arr[i].start_time = -1; 
//         arr[i].ct = 0;
//         arr[i].tat = 0;
//         arr[i].rt = 0;
//     }
// }

void createProcess(struct Process arr[], int n) {
    printf("Enter details for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process ID: ");
        scanf("%s", arr[i].pid);

        printf("Arrival Time: ");
        while (scanf("%d", &arr[i].arrival) != 1 || arr[i].arrival < 0) {
            printf("Invalid input. Enter a non-negative integer for Arrival Time: ");
            while (getchar() != '\n'); 
        }
        printf("Burst Time: ");
        while (scanf("%d", &arr[i].burst) != 1 || arr[i].burst <= 0) {
            printf("Invalid input. Enter a positive integer for Burst Time: ");
            while (getchar() != '\n'); 
        }
        arr[i].remaining_burst = arr[i].burst;
        arr[i].start_time = -1; 
        arr[i].ct = 0;
        arr[i].tat = 0;
        arr[i].rt = 0;
    }
}

void FCS(int n,struct Process arr[n]){
    quickSort(arr,0,n-1,'A');
    int time =0;
    int tt=0;
    int rt=0;
    for(int i =0;i<n;i++){
        struct Process p = arr[i];
        int start_time;
        if(time<p.arrival){
            time=p.arrival;
        }
        start_time=time;
        int c_time=time+p.burst;
        int tat = c_time - p.arrival;
        int rpt = start_time-p.arrival;
        tt+=tat;
        rt+=rpt;
        time=c_time;
        printf("%s  TAT: %d  RT: %d\n",p.pid,tat,rpt);
    }
    printf("\n");
    printf("average turnaround time : %.2f\n",(float)tt/n);
    printf("average response time: %.2f\n", (float)rt/n);
}

void RoundRobin(struct Process arr[], int n, int quantum) {
    quickSort(arr, 0, n - 1, 'A');
    int tt = 0;
    int rt = 0;
    int time = 0;
    bool completed = false;
    while (!completed) {
        completed = true;
        for (int i = 0; i < n; i++) {
            if(arr[i].remaining_burst > 0){
                completed = false; 
                if(arr[i].start_time == -1){
                    arr[i].start_time = time; 
                }
                if(arr[i].remaining_burst <= quantum){
                    time += arr[i].remaining_burst;
                    arr[i].remaining_burst = 0;
                    arr[i].ct = time;
                    arr[i].tat = arr[i].ct - arr[i].arrival;
                    arr[i].rt = arr[i].start_time - arr[i].arrival;
                    // printf("ckjnscl\n");
                    printf("%s  TAT: %d  RT: %d\n", arr[i].pid, arr[i].tat, arr[i].rt);
                    // tt += arr[i].tat;
                    // rt += arr[i].rt;
                } 
                else{
                    arr[i].remaining_burst-=quantum;
                    time += quantum;
                }
            }
        }
    }
    for(int i =0;i<n;i++){
        tt += arr[i].tat;
        rt += arr[i].rt;
    }
    printf("\n");
    printf("Average Turnaround Time: %.2f\n", (float)tt / n);
    printf("Average Response Time: %.2f\n", (float)rt / n);
}

void SJF(struct Process arr[],int n){
    quickSort(arr,0,n-1,'A');
    int time=0;
    // if(time<arr[0].arrival){
    //     time = arr[0].arrival;
    // }
    int i=0;
    bool comp = false;
    while(!comp && i<n){
        comp=true;
        for(int k=0;k<n;k++){
            comp=comp&& !arr[k].remaining_burst;
        }
        int j =i;
        while(j<n && arr[j].arrival<=time){
            j++;
        }
        quickSort(arr,i,j-1,'B');
        // struct Process p = arr[i];
        arr[i].start_time=time;
        time+=arr[i].remaining_burst;
        arr[i].remaining_burst=0;
        arr[i].ct=time;
        arr[i].tat=arr[i].ct-arr[i].arrival;
        arr[i].rt = arr[i].start_time-arr[i].arrival;
        printf("%s  TAT: %d  RT: %d\n", arr[i].pid, arr[i].tat, arr[i].rt);
        i++;
    }
    int tt=0;
    int rt=0;
    for(int k =0;k<n;k++){
        tt+=arr[k].tat;
        rt+=arr[k].rt;
    }
    printf("\n");
    printf("Average Turnaround Time: %.2f\n", (float)tt / n);
    printf("Average Response Time: %.2f\n", (float)rt / n);
}

void SRTF(struct Process arr[], int n) {
    quickSort(arr,0,n-1,'A');
    int time=0;
    // if(time<arr[0].arrival){
    //     time = arr[0].arrival;
    // }
    int i=0;
    bool comp = false;
    while(!comp && i<n){
        comp=true;
        for(int k=0;k<n;k++){
            comp=comp&& !arr[k].remaining_burst;
        }
        int j =i;
        while(j<n && arr[j].arrival<=time){
            j++;
        }
        quickSort(arr,i,j-1,'B');
        // struct Process p = arr[i];
        arr[i].start_time=time;
        time+=arr[i].remaining_burst;
        arr[i].remaining_burst=0;
        arr[i].ct=time;
        arr[i].tat=arr[i].ct-arr[i].arrival;
        arr[i].rt = arr[i].start_time-arr[i].arrival;
        printf("%s  TAT: %d  RT: %d\n", arr[i].pid, arr[i].tat, arr[i].rt);
        i++;
    }
    int tt=0;
    int rt=0;
    for(int k =0;k<n;k++){
        tt+=arr[k].tat;
        rt+=arr[k].rt;
    }
    printf("\n");
    printf("Average Turnaround Time: %.2f\n", (float)tt / n);
    printf("Average Response Time: %.2f\n", (float)rt / n);
}






int main() {
    int n;
    
    printf("Number of Processes: ");
    scanf("%d", &n);
    if(n<4){
        printf("Invalid input\n");
        exit(0);
    }
    struct Process arr[n]; 
    createProcess(arr, n);
    // struct Process arr1 = arr; 
    int quantum;
    printf("Time Quantum for Round Robin:");
    scanf("%d",&quantum);
    struct Process arr1[n], arr2[n], arr3[n],arr4[n];
    copyProcesses(arr,arr1,n);
    copyProcesses(arr,arr2,n);
    copyProcesses(arr,arr3,n);
    copyProcesses(arr,arr4,n);
    printf("\n Round Robin: \n");
    RoundRobin(arr1,n,quantum);

    printf("\n FCS:\n");
    FCS(n,arr2);

    printf("\n SJF:\n");
    SJF(arr3,n);

    printf("\n SRTF: \n");
    SRTF(arr4,n);
    return 0;
}
// 4 P1 0 5 P2 2 3 P3 1 8 P4 3 6 2