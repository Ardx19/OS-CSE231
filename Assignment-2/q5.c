#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

struct page {
    uint8_t page_number;
    bool reference_bit;
    bool dirty_bit;
};

int main() {
    int pagel[50];
    char inp[100];  
    int nframes;
    int count = 0;
    
    printf("Enter the number of frames: ");
    scanf("%d", &nframes);
    
    printf("Enter the page requests: ");
    scanf("%s", inp);
    
    int digit = 0;
    for (int i = 0; inp[i] != '\0'; i++) {
        if (inp[i] != ',') {
            digit = digit * 10 + (inp[i] - '0');
        } else {
            pagel[count++] = digit;
            digit = 0;
        }
    }
    pagel[count++] = digit;

    struct page frame[nframes];
    for (int i = 0; i < nframes; i++) {
        frame[i].page_number = -1;
        frame[i].reference_bit = 0;
        frame[i].dirty_bit = 0;
    }

    int clock = 0;
    int pageh = 0;
    int pagem = 0;
    
    for (int i = 0; i < count; i++) {
        uint8_t curpage = pagel[i];
        int result = 0;

        for (int j = 0; j < nframes; j++) {
            if (frame[j].page_number == curpage) {
                result = 1;
                frame[j].reference_bit = 1;
                break;
            }
        }

        if (result == 1) {
            pageh++;
        } else {
            pagem++;
            int b = 1;

            while (b != 0) {
                if (frame[clock].reference_bit == 0) {
                    frame[clock].page_number = curpage;
                    frame[clock].reference_bit = 1;
                    frame[clock].dirty_bit = 1;
                    clock = (clock + 1) % nframes;
                    b = 0;
                } else {
                    frame[clock].reference_bit = 0;
                    clock = (clock + 1) % nframes;
                }
            }
        }
    }

    printf("Total number of hits: %d\n", pageh);
    printf("Total number of page faults: %d\n", pagem);
    
    float per = ((float)pageh / count) * 100;
    printf("Hit percentage is: %.2f%%\n", per);

    return 0;
}