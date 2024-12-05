#include <stdio.h>
#include <stdlib.h>

#define SEEK_TIME 2
#define SECTORS_PER_TRACK 100
#define RPM 72

typedef struct Nde {
    int val;
    int track;
    int sector;
    int track_diff;
} Node;

Node q[4];
Node curr = {
    .val = 100,
    .track = 1,
    .sector = 0,
    .track_diff = 0
};

void SSTF(int n, int requestNumber) {
    for (int i = 0; i < n; i++) {
        if (q[i].track_diff == 1000) {
            continue;
        } else {
            q[i].track_diff = abs(q[i].track - curr.track);
        }
    }

    int shortest_index = 0;
    int shortest_diff = q[0].track_diff;
    for (int i = 1; i < n; i++) {
        if (q[i].track_diff < shortest_diff) {
            shortest_diff = q[i].track_diff;
            shortest_index = i;
        }
    }

    int seek_time = shortest_diff * SEEK_TIME;
    
    int dist = abs(q[shortest_index].sector - curr.sector);
    double div = (60.0 / RPM) / SECTORS_PER_TRACK;
    double final = dist * div * 1000;

    printf("Seek Time %d: %d ms\n", requestNumber, seek_time);
    printf("Rotational Latency %d: %.2f ms\n", requestNumber, final);

    curr.track = q[shortest_index].track;
    curr.sector = q[shortest_index].sector;
    curr.val = q[shortest_index].val;
    q[shortest_index].track_diff = 1000;
}

int main() {
    printf("Disk requests: ");
    for (int i = 0; i < 4; i++) {
        int x;
        scanf("%d", &x);
        q[i].val = x;
        q[i].track = q[i].val / curr.val;
        q[i].sector = q[i].val % curr.val;
    }

    for (int i = 0; i < 4; i++) {
        SSTF(4, i + 1);
    }

    return 0;
}
