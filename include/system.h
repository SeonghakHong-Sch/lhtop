#pragma once

#include <stdio.h>
#include "common.h"

typedef struct {
    //total cpu
    TIME total_cpu;
    TIME idle_cpu;
    
    //each cores
    unsigned int cores;
    TIME* core_total;
    TIME* core_idle;
    //TIME* core_
    

    //mem
    TIME total_mem;
    TIME avail_mem;
    //TIME avail_mem; 
    TIME swap_total;
    TIME swap_free;
    

} SystemInfo;

void set_system_info(SystemInfo* system);