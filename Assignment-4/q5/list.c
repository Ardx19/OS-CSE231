#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

char *FILE_NAME;

int ls(){
    DIR* dir = opendir(FILE_NAME);
    if(dir==NULL){
        return 1;
    }

    struct dirent* entity;
    entity = readdir(dir);
    while(entity!=NULL){
        printf("%s\n",entity->d_name);
        entity = readdir(dir);
    }

    closedir(dir);
    return 0;
}

int main(int argc, char **argv){
    if(argc==1){
        FILE_NAME=malloc(2*sizeof(char));
        strcpy(FILE_NAME,".");
    }
    else{
        FILE_NAME=malloc((strlen(argv[1]) + 1) * sizeof(char));
        strcpy(FILE_NAME,argv[1]);
    }
    if(ls()){
        printf("ls: cannot access %s: No such file or directory\n",FILE_NAME);
    }
    return 0;
}
