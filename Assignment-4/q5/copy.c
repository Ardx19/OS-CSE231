#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

char *FILE_NAME;
char *DESTINATION;

void cp() {
    int source = open(FILE_NAME, O_RDONLY);
    if (source < 0) {
        perror("Error opening source file");
        return;
    }

    char *file = strrchr(FILE_NAME, '/');
    if(file != NULL){
        file = file + 1; 
    } 
    else{
        file = FILE_NAME;
    }
    char *destination_path = malloc(strlen(DESTINATION) + strlen(file) + 2);
    if (!destination_path) {
        perror("Memory allocation failed");
        close(source);
        return;
    }
    strcpy(destination_path, DESTINATION);
    if (destination_path[strlen(destination_path) - 1] != '/') {
        strcat(destination_path, "/");
    }
    strcat(destination_path, file);

    //added for overwriting
    if (access(destination_path, F_OK) == 0) {
        printf("cp: overwrite '%s'? (y/n): ", destination_path);
        char response = getchar();
        if (response != 'y' && response != 'Y') {
            close(source);
            free(destination_path);
            return;
        }
    }

    //opening file in destination folder
    int dest = open(destination_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest < 0) {
        perror("Error opening destination file");
        close(source);
        free(destination_path);
        return;
    }
    
    //all the copy pasting
    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(source, buffer, sizeof(buffer))) > 0) {
        if (write(dest, buffer, bytes_read) != bytes_read) {
            perror("Error writing to destination file");
            close(source);
            close(dest);
            free(destination_path);
            return;
        }
    }
    if (bytes_read < 0) {
        perror("Error reading source file");
    }

    close(source);
    close(dest);
    free(destination_path);
}

int main(int argc, char **argv){
    if(argc==3){
        FILE_NAME = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
        DESTINATION=(char*)malloc((strlen(argv[2])+1)*sizeof(char));
        strcpy(FILE_NAME,argv[1]);
        strcpy(DESTINATION,argv[2]);
        cp();
    }
    else{
        if(argc == 1){
            printf("cp: missing file operand\n");
        }
        else{
            FILE_NAME = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
            strcpy(FILE_NAME,argv[1]);
            printf("cp: missing destination file operand after '%s'\n",FILE_NAME);
            free(FILE_NAME);
        }
        return 1;
    }
    return 0;
}
