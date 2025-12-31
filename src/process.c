#include <stdio.h>
#include "common.h"
#include "process.h"

void set_process_info(ProcessList* process_list) {
    FILE* proc_stream;
    char path[BUF_SIZE];
    for (int i = 0; i < process_list->count; i++) {
        sprintf(path, "/proc/%d/stat", process_list->processes[i]);
        proc_stream = fopen(path, "r");
        if (!proc_stream) continue; //파일 정상 체크
        //파일 파싱 ~~


        fclose(proc_stream);
    }
}