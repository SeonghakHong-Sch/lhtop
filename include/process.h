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

typedef struct {
    int count; //프로세스 갯수
    ProcessInfo* processes; //리스트

} ProcessList;

void set_process_info(ProcessInfo* process_list);

void scan_all_process(ProcessList* process_list);