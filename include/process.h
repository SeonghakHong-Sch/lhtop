#include <stdio.h>
#include "common.h"

typedef struct {
    //process info
    int pid;
    int ppid;
    int uid;
    char comm[BUF_SIZE]; //process name
    char stat;
    long priority;

    //cpu
    TIME utime;
    TIME stime;

    //mem
    long vm_rss;

} ProcessInfo;

void set_process_info(ProcessInfo* process);