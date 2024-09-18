#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>


int main(){
    if(!fork()){
        char *args[] = {"./date",NULL};
        execvp(args[0],args);
    }
    else{
        wait(NULL);
        if(!fork()){
            char *args[] = {"./cal",NULL};
            execvp(args[0],args);
        }
        else{
            wait(NULL);
            if(!fork()){
                char *args[] = {"./uptime",NULL};
                execvp(args[0],args);
            }
            else{
                wait(NULL);
                printf("\n\nEnd of main\n");
            }
        }
    }
    return 0;
}