#include <stdio.h>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo info;

    if (sysinfo(&info) == 0) {
        // info.uptime
        int sec = info.uptime%60;
        int min =(info.uptime%3600)/60;
        int hours = info.uptime/3600;
        printf("Uptime %d:%d:%d\n",hours,min,sec);
    } else {
        perror("sysinfo");
    }

    return 0;
}
