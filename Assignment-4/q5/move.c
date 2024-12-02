#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>

char *DIR_NAME;
char *DESTINATION;

// void mv(){
//     char *dest = strrchr(DIR_NAME, '/');
//     if(dest != NULL){
//         dest = dest + 1; 
//     } 
//     else{
//         dest = DIR_NAME;
//     }
//     char *destination_path = malloc(strlen(DESTINATION) + strlen(dest) + 2);
//     if (!destination_path) {
//         perror("Memory allocation failed");
//         return;
//     }
//     strcpy(destination_path, DESTINATION);
//     if (destination_path[strlen(destination_path) - 1] != '/') {
//         strcat(destination_path, "/");
//     }
//     strcat(destination_path, dest);
//     rename(DIR_NAME,destination_path);
// }

void mv() {
    DIR *dir = opendir(DIR_NAME);
    if (!dir) {
        perror("Error opening source directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char source_path[4096];
        char dest_path[4096];
        snprintf(source_path, sizeof(source_path), "%s/%s", DIR_NAME, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", DESTINATION, entry->d_name);

        if (rename(source_path, dest_path) == -1) {
            perror("Error moving file");
            break;
        }
    }

    closedir(dir);
    rmdir(DIR_NAME);
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
    free(DIR_NAME);
    free(DESTINATION);
    return 0;
}
