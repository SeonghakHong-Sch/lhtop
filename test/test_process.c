// process.c 모듈테스트
#include <stdio.h>
#include <stdlib.h>
#include "../include/process.h"

int main() {
    ProcessList process_list = {0};

    printf("=== Scanning all processes ===\n");
    scan_all_pid(&process_list);
    printf("Total processes found: %d\n", process_list.count);
    printf("Max capacity: %d\n", process_list.max_processes);

    printf("\n=== Setting process information ===\n");
    set_process_info(&process_list);

    printf("\n=== Process List (First 10 processes) ===\n");
    int display_count = process_list.count > 10 ? 10 : process_list.count;
    for (int i = 0; i < display_count; i++) {
        ProcessInfo* p = &process_list.processes[i];
        printf("PID: %d, Name: %s, State: %c, PPID: %d\n",
               p->pid, p->comm, p->state, p->ppid);
        printf("  CPU - utime: %lu, stime: %lu\n",
               p->utime, p->stime);
        printf("  Memory - RSS: %ld kB\n\n",
               p->rss);
    }

    printf("\n=== Process List (Running processes) ===\n");
    for (int i = 0; i < process_list.count; i++) {
        ProcessInfo* p = &process_list.processes[i];
        if (p->state == 'R') {
            printf("PID: %d, Name: %s, State: %c, PPID: %d\n",
                p->pid, p->comm, p->state, p->ppid);
            printf("  CPU - utime: %lu, stime: %lu\n",
                p->utime, p->stime);
            printf("  Memory - RSS: %ld kB\n\n",
                p->rss);
        }
    }

    printf("=== Process Statistics ===\n");
    int running = 0, sleeping = 0, stopped = 0, zombie = 0, other = 0;
    for (int i = 0; i < process_list.count; i++) {
        switch (process_list.processes[i].state) {
            case 'R': running++; break;
            case 'S': sleeping++; break;
            case 'T': stopped++; break;
            case 'Z': zombie++; break;
            default: other++; break;
        }
    }
    printf("Running: %d\n", running);
    printf("Sleeping: %d\n", sleeping);
    printf("Stopped: %d\n", stopped);
    printf("Zombie: %d\n", zombie);
    printf("Other: %d\n", other);

    // 메모리 해제
    free(process_list.processes);
    printf("\n=== Process Test Finish ===\n");
    return 0;
}
