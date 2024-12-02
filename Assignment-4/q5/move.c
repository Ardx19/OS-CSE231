#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>

char *DIR_NAME;
char *DESTINATION;

void mv(){
    char *dest = strrchr(DIR_NAME, '/');
    if(dest != NULL){
        dest = dest + 1; 
    } 
    else{
        dest = DIR_NAME;
    }
    char *destination_path = malloc(strlen(DESTINATION) + strlen(dest) + 2);
    if (!destination_path) {
        perror("Memory allocation failed");
        return;
    }
    strcpy(destination_path, DESTINATION);
    if (destination_path[strlen(destination_path) - 1] != '/') {
        strcat(destination_path, "/");
    }
    strcat(destination_path, dest);
    rename(DIR_NAME,destination_path);
}

int main(int argc, char **argv){
    if(argc!=3){
        if(argc == 1){
            printf("mv: missing directory operand\n");
        }
        else{
            DIR_NAME = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
            strcpy(DIR_NAME,argv[1]);
            printf("cp: missing destination directory operand after '%s'\n",DIR_NAME);
            free(DIR_NAME);
        }
        return 1;
    }
    DIR_NAME = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
    DESTINATION=(char*)malloc((strlen(argv[2])+1)*sizeof(char));
    strcpy(DIR_NAME,argv[1]);
    strcpy(DESTINATION,argv[2]);
    mv();
    return 0;
}
