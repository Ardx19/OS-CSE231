#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void print_default_time() {
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    char buffer[100];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    printf("Default local date and time: %s\n", buffer);
}

void print_utc_time() {
    time_t current_time = time(NULL);
    struct tm *utc_time = gmtime(&current_time);
    char buffer[100];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S UTC", utc_time);
    printf("UTC date and time: %s\n", buffer);
}

void print_rfc2822_time() {
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    char buffer[100];

    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %z", local_time);
    printf("RFC 2822 format: %s\n", buffer);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_default_time();
    } 
    else if (argc == 2){
        if (strcmp(argv[1], "-u") == 0) {
            print_utc_time();
        } else if (strcmp(argv[1], "-r") == 0) {
            print_rfc2822_time();
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[1]);
            printf("Usage: %s [-u for UTC] [-r for RFC 2822 format]\n", argv[0]);
            return 1;
        }
    } else {
        fprintf(stderr, "Too many arguments.\n");
        printf("Usage: %s [-u for UTC] [-r for RFC 2822 format]\n", argv[0]);
        return 1;
    }

    return 0;
}
