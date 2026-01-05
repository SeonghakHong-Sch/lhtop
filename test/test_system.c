// system.c 모듈테스트
#include <stdio.h>
#include <stdlib.h>
#include "../include/system.h"

int main() {
    SystemInfo sys;
    set_system_info(&sys);
    
    printf("=== System CPU Info ===\n");
    printf("Total CPU time: %lu\n", sys.total_cpu);
    printf("Idle CPU time: %lu\n", sys.idle_cpu);
    printf("Number of cores: %d\n", sys.cores);
    
    printf("\n=== Per-Core CPU Info ===\n");
    for (int i = 0; i < sys.cores; i++) {
        printf("Core %d - Total: %lu, Idle: %lu\n", 
               i, sys.core_total[i], sys.core_idle[i]);
    }
    
    printf("\n=== Memory Info ===\n");
    printf("Total Memory: %lu kB\n", sys.total_mem);
    printf("Free Memory: %lu kB\n", sys.avail_mem);
    printf("Swap Total: %lu kB\n", sys.swap_total);
    printf("Swap Free: %lu kB\n", sys.swap_free);
    
    // 메모리 해제
    free(sys.core_total);
    free(sys.core_idle);
    printf("=== Systme Test Finish ===");
    return 0;
}