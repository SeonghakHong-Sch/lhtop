#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "process.h"
#include "system.h"
#include "stats.h"

void cal_system_stat(SystemStats* result, SystemInfo* prev_sys, SystemInfo* curr_sys) {
    //total cpu
    TIME d_t_cpu = curr_sys->total_cpu - prev_sys->total_cpu;
    TIME d_i_cpu = curr_sys->idle_cpu - prev_sys->idle_cpu;

    result->total_cpu_percent = (d_t_cpu - d_i_cpu) / (float) d_t_cpu;

    //cores
    for (int i = 0; i < curr_sys->cores; i++) {
        TIME d_t_core = curr_sys->core_total[i] - prev_sys->core_total[i];
        TIME d_i_core = curr_sys->core_idle[i] - prev_sys->core_idle[i];
        result->cores_cpu_percent[i] = (d_t_core - d_i_core) / (float) d_t_core;
    }

    //mem
    result->mem_percent = (curr_sys->total_mem - curr_sys->free_mem) / (float) curr_sys->total_mem;
    //swap
    result->mem_percent = (curr_sys->swap_total - curr_sys->swap_free) / (float) curr_sys->swap_total;
}

void cal_proc_stat(ProcessStats result[], ProcessList* prev_procs, ProcessList* curr_procs, SystemInfo* sys) {    
    if (curr_procs->count <= 5000) { //process undter 5000, linear matching
        for (int i = 0; i < curr_procs->count; i++) {
            ProcessInfo* curr_proc = &curr_procs->processes[i];
            int PID = curr_proc->pid;
            ProcessInfo* prev_proc = search_proc_by_pid(prev_procs, PID);
            
            if (!prev_proc) {
                result[i].pid = PID;
                result[i].cpu_percent = 0.0;
                result[i].mem_percent = (curr_proc->rss) / (float) sys->total_mem * PAGE_SIZE;
                continue;
            }
            
            TIME d_u_time = curr_proc->utime - prev_proc->utime;
            TIME d_s_time = curr_proc->stime - prev_proc->stime;
            TIME d_total_time = d_u_time + d_s_time; //delta time while 1 sec

            result[i].pid = PID;
            result[i].cpu_percent = (d_total_time) / (float) JIFFIES;
            result[i].mem_percent = (curr_proc->rss * PAGE_SIZE) / (float) sys->total_mem;
            
        }
    } else {
        //in heavy system, it will be hashmap...
    }

}

ProcessInfo* search_proc_by_pid(ProcessList* prev_procs, int pid) {
    for (int i = 0; i < prev_procs->count; i++) {
        if (pid == prev_procs->processes[i].pid) {
            return &prev_procs->processes[i];
        }
    }
    return NULL; //c에 nullptr 없음, 이거 걍 0임;;
}