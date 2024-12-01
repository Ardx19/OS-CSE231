#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<ctype.h>
#include<stdlib.h>


char *FILE_NAME;
int saved_stdin;
int fd;

int wc(){
    fd = open(FILE_NAME,O_RDONLY);
    saved_stdin=STDIN_FILENO;
    dup2(fd,STDIN_FILENO);
    if(fd==-1){
        return 1;       
    }

    int lines =0,words=0,bytes=0;
    char c , prev = '\0';
    while(scanf("%c",&c)==1){
        bytes++;
        if(c=='\n'){
            lines++;
        }
        if(isspace(c) && !isspace(prev)){
            words++;
        }
        prev=c;
    }
    if(!isspace(prev)){
        words++;
    }
    // printf("lines= %d words= %d bytes= %d %s\n",lines,words,bytes,FILE_NAME);
    printf("%d %d %d %s\n",lines,words,bytes,FILE_NAME);
    return 0;
}

int main(int argc, char **argv){
    if(argc==1){
        printf("wc: No file given\n");
        return 0;
    }
    else{
        FILE_NAME = (char*)malloc((strlen(argv[1]) + 1) * sizeof(char));
        strcpy(FILE_NAME,argv[1]);
    }
    int err = wc();
    dup2(saved_stdin,fd);
    close(saved_stdin);
    if(err){
        printf("wc: %s: No such file or directoy\n",FILE_NAME);
    }
}
