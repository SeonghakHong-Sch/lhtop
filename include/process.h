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
    unsigned long utime;
    unsigned long stime;

    //mem
    long vm_rss;

    

} ProcessInfo;