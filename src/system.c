#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "system.h"

void set_system_info(SystemInfo* system) {
    int CORES = sysconf(_SC_NPROCESSORS_ONLN);
    system->cores = CORES;

    FILE* sys_stream;
    char line[BUF_SIZE];

    //total cpu
    sys_stream = fopen("/proc/stat", "r");
    unsigned long cpu_time[7];

    fscanf(sys_stream, "cpu %lu %lu %lu %lu %lu %lu %lu",
        &cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
        &cpu_time[4], &cpu_time[5], &cpu_time[6]);
    
    system->total_cpu = 0;
    for (int i = 0; i < 7; i++) {
        system->total_cpu += cpu_time[i];
    }
    system->idle_cpu = cpu_time[3];

    for (int i = 0; i < CORES; i++) {
        fgets(line, BUF_SIZE, sys_stream);
        sscanf(line, "cpu%*d %lu %lu %lu %lu %lu %lu %lu ",
            &cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
            &cpu_time[4], &cpu_time[5], &cpu_time[6]);
        
        for (int i = 0; i < 7; i++) {
            //동적할당
        }
    }
    fclose(sys_stream);

    //Mem
    sys_stream = fopen("/proc/meminfo", "r");
    
    while (fgets(line, BUF_SIZE, sys_stream)) {
        if (sscanf(line, "MemTotal: %lu kB", &system->total_mem) != -1) continue;
        if (sscanf(line, "MemFree: %lu kB", &system->free_mem) != -1) continue;
        if (sscanf(line, "SwapTotal: %lu kB", &system->swap_total) != -1) continue;
        if (sscanf(line, "SwapFree: %lu kB", &system->swap_free) != -1) continue;
    }


    
    


}
