#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#define HASH_SIZE 100003
#define FILE_NAME "q1-50mil.txt"

clock_t start,end;

typedef struct Node{
    char station_name[100];
    double min;
    double max;
    double sum;
    int count;
    struct Node* next;
}Node;

Node* map[HASH_SIZE]={NULL};

// used DJB2 hashing technique 
int hash(const char* station_name){
    unsigned long hash = 5381;
    int c;
    while((c=*station_name++)){
        hash = ((hash<<5)+hash)+c;
    }
    return hash%HASH_SIZE;
}

int update_map(char *station_name,double temp){
    int key = hash(station_name);
    Node* curr = map[key];
    Node* prev=curr;
    while(curr){
        if(strcmp(curr->station_name,station_name)==0){
            if(curr->min > temp){
                curr->min=temp;
            }
            if(curr->max<temp){
                curr->max=temp;
            }
            curr->sum+=temp;
            curr->count++;
            return 1;
        }
        prev = curr;
        curr=curr->next;
    }
    Node* dummy = (Node*)malloc(sizeof(Node));
    strcpy(dummy->station_name,station_name);
    dummy->min=temp;
    dummy->max=temp;
    dummy->sum=temp;
    dummy->count=1;
    if(prev){
        prev->next = dummy;
    }
    else{
        map[key] = dummy;
    }
    dummy->next=NULL;
    return 0;
}

void freeSpace(){
    for (int i = 0; i < HASH_SIZE; i++) {
        Node *curr = map[i];
        while (curr) {
            Node *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
}

void print(){
    for(int i=0;i<HASH_SIZE;i++){
        Node* curr = map[i];
        while(curr){
            double mean = curr->sum/curr->count;
            printf("%s min = %.2lf mean = %.2lf max = %.2lf\n",curr->station_name,curr->min,mean,curr->max);
            curr=curr->next;
        }
    }
}


int part1(){
    start=clock();
    double temp;
    char station_name[100];
    int fd = open(FILE_NAME, O_RDONLY);
    if(fd == -1){
        perror("Error opening files");
    }
    //Get file size
    off_t size = lseek(fd,0,SEEK_END);
    if(size==-1){
        printf("Error using lseek command\n");
        close(fd);
        return 1;
    }

    char *map = mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd,0);
    if(map==MAP_FAILED){
        perror("Error mapping file\n");
        close(fd);
        return 1;
    }

    madvise(map, size, MADV_SEQUENTIAL);

    int i=0;
    while(i<size){
        int j=0;
        while(i<size && map[i]!=';' && map[i]!='\n'){
            if(j<sizeof(station_name)-1){
                station_name[j++]=map[i];
            }
            i++;
        }
        station_name[j]='\0';

        if(i<size && map[i]==';'){
            i++;
        }

        char temp_str[50];
        j=0;
        while(i<size && map[i]!='\n'){
            if(j<sizeof(temp_str)-1){
                temp_str[j++]=map[i];
            }
            i++;
        }
        temp_str[j]='\0';
        temp = atof(temp_str);
        if(i<size && map[i]=='\n'){
            i++;
        }
        update_map(station_name,temp);
    }
    munmap(map,size);
    close(fd);
    print();
    end=clock();
    double time = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Time time taken by part1 using mmap: %f\n",time);
    freeSpace();
}

void resetMap() {
    for (int i = 0; i < HASH_SIZE; i++) {
        map[i] = NULL;
    }
}

int part2(){
    start = clock();
    FILE* file = fopen(FILE_NAME,"r+");
    if(file==NULL){
        printf("Error in opening file\n");
        return 1;
    }
    double temp;
    char station_name[100];
    while(fscanf(file,"%99[^;];%lf\n",station_name,&temp)==2){ // 99 somehow help in the buffer not overflowing
        update_map(station_name,temp);
    }
    // print();
    fclose(file);
    end = clock();
    double time = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Time time taken by part2 using scanf: %f\n",time);
    freeSpace();
    return 0;
}

int main(){
    if(part1()!=0){
        return 1;
    }
    resetMap();
    if(part2()!=0){
        return 1;
    }
    /*Both parts work fine on their own but when called together they cause a segmentation fault maybe due to 
      memory overflow
      Added resetMap() which reinitializes the buket array for map*/
    return 0;
}
