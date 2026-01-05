#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "common.h"
#include "process.h"

void set_process_info(ProcessList* process_list) {
    FILE* proc_stream;
    char path[BUF_SIZE];
    ProcessInfo* processes = process_list->processes;

    for (int i = 0; i < process_list->count; i++) {
        sprintf(path, "/proc/%d/stat", processes[i].pid);
        proc_stream = fopen(path, "r");
        if (!proc_stream) continue; //파일(프로세스) 정상 체크
        
        //파일 파싱 ~~
        fscanf(proc_stream, "%d (%[^)]) %c %d %*d %*d %*d %*d %*d %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %*d %*d %*d %*d %lu",
            &processes[i].pid,
            processes[i].comm,
            &processes[i].state,
            &processes[i].ppid,
            &processes[i].utime,
            &processes[i].stime,
            &processes[i].rss
        );
        fclose(proc_stream);
    }
}

void scan_all_pid(ProcessList* process_list) {
    char* path = "/proc";
    DIR* proc_dir = opendir(path);
    int i = 0;

    int* max_processes = &process_list->max_processes; //변수 명 너무 길어서...

    if (process_list->processes == NULL) {
        process_list->processes =(ProcessInfo*) malloc(sizeof(ProcessInfo)* INITIAL_CAPACITY);
        *max_processes = INITIAL_CAPACITY;
    }
    
    struct dirent* dient;
    while ((dient = readdir(proc_dir))) {
        if (!isdigit(dient->d_name[0])) continue; //프로세스 directory만 검거
        process_list->processes[i].pid = atoi(dient->d_name);
        i++;

        //capacity 동적재할당
        if (i == *max_processes && *max_processes < MAX_CAPACITY) {
            *max_processes *= 2;
            process_list->processes = realloc(process_list->processes, sizeof(ProcessInfo)* *max_processes);
        }
    }
    process_list->count = i;

    closedir(proc_dir);
}