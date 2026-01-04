#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "process.h"
#include "system.h"

typedef struct {
    //cpu
    float total_cpu_percent;
    float* cores_cpu_percent;

    //mem
    float mem_percent;
    float swap_percent;
} SystemStats;

typedef struct {
    int pid;

    //cpu
    float cpu_percent;

    //mem
    float mem_percent;

} ProcessStats;

void cal_system_stat(SystemStats* result, SystemInfo* prev_sys, SystemInfo* curr_sys);

void cal_proc_stat(ProcessStats result[], ProcessList* prev_procs, ProcessList* curr_procs, SystemInfo* sys);

ProcessInfo* search_proc_by_pid(ProcessList* prev_procs, int pid);