// stats.c 모듈테스트
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/stats.h"

int main() {
    SystemInfo prev_sys, curr_sys;
    ProcessList prev_procs = {0}, curr_procs = {0};

    printf("=== Collecting initial system and process data ===\n");
    set_system_info(&prev_sys);
    scan_all_pid(&prev_procs);
    set_process_info(&prev_procs);

    printf("Waiting 1 second...\n");
    sleep(1);

    printf("\n=== Collecting current system and process data ===\n");
    set_system_info(&curr_sys);
    scan_all_pid(&curr_procs);
    set_process_info(&curr_procs);

    // SystemStats 초기화
    SystemStats sys_stats;
    sys_stats.cores_cpu_percent = (float*)malloc(sizeof(float) * curr_sys.cores);

    printf("\n=== Calculating system statistics ===\n");
    cal_system_stat(&sys_stats, &prev_sys, &curr_sys);

    printf("Total CPU Usage: %.2f%%\n", sys_stats.total_cpu_percent * 100);
    printf("Memory Usage: %.2f%%\n", sys_stats.mem_percent * 100);
    printf("Swap Usage: %.2f%%\n", sys_stats.swap_percent * 100);

    printf("\n=== Per-Core CPU Usage ===\n");
    for (int i = 0; i < curr_sys.cores; i++) {
        printf("Core %d: %.2f%%\n", i, sys_stats.cores_cpu_percent[i] * 100);
    }

    // ProcessStats 초기화
    ProcessStats* proc_stats = (ProcessStats*)malloc(sizeof(ProcessStats) * curr_procs.count);

    printf("\n=== Calculating process statistics ===\n");
    cal_proc_stat(proc_stats, &prev_procs, &curr_procs, &curr_sys);

    printf("Total processes analyzed: %d\n", curr_procs.count);

    printf("\n=== Top 10 CPU consuming processes ===\n");
    // CPU 사용률로 정렬하여 상위 10개 출력
    int display_count = curr_procs.count > 10 ? 10 : curr_procs.count;
    for (int i = 0; i < display_count; i++) {
        float max_cpu = 0.0;
        int max_idx = -1;
        for (int j = 0; j < curr_procs.count; j++) {
            if (proc_stats[j].cpu_percent > max_cpu) {
                max_cpu = proc_stats[j].cpu_percent;
                max_idx = j;
            }
        }
        if (max_idx != -1) {
            ProcessInfo* p = &curr_procs.processes[max_idx];
            printf("PID: %d, Name: %s, CPU: %.2f%%, MEM: %.2f%%\n",
                   proc_stats[max_idx].pid, p->comm,
                   proc_stats[max_idx].cpu_percent * 100,
                   proc_stats[max_idx].mem_percent * 100);
            proc_stats[max_idx].cpu_percent = -1.0; // 다음 반복에서 제외
        }
    }

    // 메모리 해제
    free(sys_stats.cores_cpu_percent);
    free(proc_stats);
    free(prev_sys.core_total);
    free(prev_sys.core_idle);
    free(curr_sys.core_total);
    free(curr_sys.core_idle);
    free(prev_procs.processes);
    free(curr_procs.processes);

    printf("\n=== Stats Test Finish ===\n");
    return 0;
}
