#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    M = 1, D, ML, DL, MR, DR, PM, PD
} Description;

typedef struct {
    char sign[15];
    char lane[15];
    int seconds;
    int priority;
} Activity;

typedef struct {
    Activity activity;
    int prio;
    int time;
} TrafficData;

typedef struct {
    TrafficData data[20];
    int count;
} Heap;

typedef struct {
    Activity activity;
    int seconds;
    int priority;
} Lane;

void initHeap(Heap *h) {
    h->count = 0;
}

Description determine(const char *desc) {
    if (strcmp(desc, "M") == 0) return M;
    if (strcmp(desc, "D") == 0) return D;
    if (strcmp(desc, "ML") == 0) return ML;
    if (strcmp(desc, "DL") == 0) return DL;
    if (strcmp(desc, "MR") == 0) return MR;
    if (strcmp(desc, "DR") == 0) return DR;
    if (strcmp(desc, "PM") == 0) return PM;
    if (strcmp(desc, "PD") == 0) return PD;
    return M; // default
}

const char *convertEnum(Description desc) {
    switch (desc) {
        case M: return "M";
        case D: return "D";
        case ML: return "ML";
        case MR: return "MR";
        case DL: return "DL";
        case DR: return "DR";
        case PM: return "PM";
        case PD: return "PD";
        default: return "Unknown";
    }
}

TrafficData createData(char *desc, int time) {
    TrafficData td;
    td.activity.prio = determine(desc);
    td.activity.time = time;
    return td;
}

bool enqueue(Heap *hp, TrafficData td) {
    if (hp->count >= 20) return false;
    int curr = hp->count;
    int parent;
    while (curr > 0 && td.prio < hp->data[(curr - 1) / 2].prio) {
        parent = (curr - 1) / 2;
        hp->data[curr] = hp->data[parent];
        curr = parent;
    }
    hp->data[curr] = td;
    hp->count++;
    return true;
}

TrafficData dequeue(Heap *hp) {
    TrafficData last;
    if (hp->count == 0) {
        last = createData("Unknown", 0);
        return last;
    }
    last = hp->data[hp->count - 1];
    (hp->count)--;
    int index = 0, lesser;
    while (index < hp->count) {
        int left = (2 * index) + 1;
        if (left >= hp->count) break;
        if (left + 1 < hp->count && hp->data[left + 1].prio < hp->data[left].prio) {
            lesser = left + 1;
        } else {
            lesser = left;
        }
        if (last.prio <= hp->data[lesser].prio) {
            break;
        }
        hp->data[index] = hp->data[lesser];
        index = lesser;
    }
    hp->data[index] = last;
    return last;
}

void displayAllData(Heap *trafficList) {
    printf("_________________________\n");
    for (int i = 0; i < trafficList->count; ++i) {
        printf("%15s - %d - %d\n", convertEnum(trafficList->data[i].desc), trafficList->data[i].time, trafficList->data[i].prio);
    }
    printf("_________________________\n");
}

int getTotalTimeBeforePedestrianCross(Heap *trafficList) {
    int totalTime = 0;
    int i = 0;
    while (i < trafficList->count && trafficList->data[i].desc != PM) {
        totalTime += trafficList->data[i].time;
        i++;
    }
    return totalTime;
}

int main() {
    Heap trafficList;
    initHeap(&trafficList);

    char sign[15];
    char lane[15];
    int seconds, priority;
    char desc[15];
    int time;

    printf("Enter traffic data (Description, Time): ");
    while (scanf("%s %d", desc, &time) == 2) {
        TrafficData td = createData(desc, time);
        if (!enqueue(&trafficList, td)) {
            printf("Queue is full!\n");
            break;
        }
    }

    printf("\nTraffic Data:\n");
    displayAllData(&trafficList);

    int totalTime = getTotalTimeBeforePedestrianCross(&trafficList);
    printf("Total time before main pedestrian can cross: %d\n", totalTime);

    return 0;
}
