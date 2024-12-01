#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<time.h>
#include<unistd.h>


char FILE_NAME[256];

void split_string(const char *input, char *arr[], int *count) {
    char *temp = strdup(input); 
    if (temp == NULL) {
        perror("Failed to allocate memory");
        return;
    }

    char *token = strtok(temp, " ");
    *count = 0;

    while (token != NULL) {
        arr[*count] = strdup(token);
        if (arr[*count] == NULL) {
            perror("Failed to allocate memory");
            free(temp);
            return;
        }
        (*count)++;
        token = strtok(NULL, " ");
    }
    arr[*count] = NULL;
    free(temp); 
}

void p1(){
    srand(time(0));
    int fd = open(FILE_NAME,O_WRONLY| O_CREAT | O_TRUNC, 0777);
    if(fd==-1){
        printf("Error in file name\n");
        return;
    }
    // printf("This is printed to terminal\n");
    int savedSTDOUT = dup(STDOUT_FILENO);
    int file = dup2(fd,STDOUT_FILENO);
    if(file==-1){
        perror("dup2 failed\n");
        close(fd);
        return;
    }


    printf("This is printed to file");
    close(fd);


    // if(dup2(savedSTDOUT,STDOUT_FILENO)==-1){
    //     perror("Failed to restore stdout");
    //     close(savedSTDOUT);
    //     return;
    // }
    // close(savedSTDOUT);
    // printf("Back to normal\n");
}

void p2(const char *command){
    int fd = open(FILE_NAME,O_WRONLY| O_CREAT | O_TRUNC, 0777);
    if(fd==-1){
        printf("Error in file name\n");
        return;
    }
    //stdout to fd
    int savedSTDOUT = dup(STDOUT_FILENO);
    int savedSTDERR = dup(STDERR_FILENO);
    int file = dup2(fd,STDOUT_FILENO);
    if(file==-1){
        perror("dup2 failed\n");
        close(fd);
        return;
    }
    //stderr to fd
    file = dup2(fd, STDERR_FILENO);
    if (file == -1) {
        perror("dup2 failed for stderr");
        close(savedSTDOUT);
        close(savedSTDERR);
        close(fd);
        return;
    }
    close(fd);

    char* arr[10];
    int count =0;
    split_string(command,arr,&count);

    execvp(
        "ls",
        arr
    );
    
}

int main(int argc, char **argv){
    if(argc == 3 && strcmp(argv[1],"-p1")==0){
        strcpy(FILE_NAME,argv[2]);
        p1();
    }
    else if(argc==4 && strcmp(argv[1],"-p2")==0){
        strcpy(FILE_NAME,argv[3]);
        const char* command = argv[2];
        p2(command);
    }
    else{
        printf("Incorrect input format\n");
        return 1;
    }
    return 0;
}
