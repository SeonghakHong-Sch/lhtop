#include <stdio.h>
#include "common.h"

typedef struct {
    //total cpu
    unsigned long total_cpu;
    unsigned long idle_cpu;
    
    //each cores
    unsigned int cores;
    unsigned long* core_total;
    unsigned long* core_idle;
    //unsigned long* core_
    

    //mem
    unsigned long total_mem;
    unsigned long free_mem;
    //unsigned long avail_mem; 
    unsigned long swap_total;

} SystemInfo;
