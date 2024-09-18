#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/wait.h>

#define CHILD_NUM 7
#define CHILD_VAL 4

int main(){
	for(int i=0;i<CHILD_NUM;i++){
		pid_t pid;
		pid = fork();
		if(pid<0){
			exit(0);
		}
		else if(!pid){
			srand(time(0)+getpid());
			int sum=0;
			for(int i=0;i<CHILD_VAL;i++){
				sum+= rand()%100+1;
			}
			int mean = sum/CHILD_VAL;
			printf("Mean is : %d\n",mean);
			exit(0);
		}
	}
	for(int i =0;i<CHILD_NUM;i++){
        wait(NULL);
    }
	return 0;
}
