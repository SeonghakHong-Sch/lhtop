#include "display.h"
#include "stats.h"
#include "process.h"
#include "system.h"
#include "common.h"

int main()
{
    init_display(); // ncurses 초기화

    SystemInfo prev_sys, curr_sys;
    ProcessList prev_procs = {0}, curr_procs = {0};
    SystemStats sys_stats;
    ProcessStats *proc_stats;

    // 초기(이전, prev) 데이터 수집
    set_system_info(&prev_sys);
    scan_all_pid(&prev_procs);
    set_process_info(&prev_procs);

    while (1)
    {
        sleep(CUSTOM_CLK); // 초 대기

        // 현재(curr) 데이터 수집
        set_system_info(&curr_sys);
        scan_all_pid(&curr_procs);
        set_process_info(&curr_procs);

        // 통계 계산
        sys_stats.cores_cpu_percent = (float*) malloc(sizeof(float) * curr_sys.cores);
        cal_system_stat(&sys_stats, &prev_sys, &curr_sys);

        proc_stats = (ProcessStats*)malloc(sizeof(ProcessStats) * curr_procs.count);
        cal_proc_stat(proc_stats, &prev_procs, &curr_procs, &prev_sys, &curr_sys);

        // 화면 렌더링
        render_display(&sys_stats, &curr_sys, proc_stats, &curr_procs);

        // 'q' 누르면 종료
        int ch = getch();
        if (ch == 'q')
            break;

        // 메모리 정리 및 prev ← curr 복사
        ProcessInfo* temp = prev_procs.processes;
        prev_procs.processes = curr_procs.processes;
        curr_procs.processes = NULL;
        free(temp);

        prev_procs.count = curr_procs.count;
        prev_sys = curr_sys;


        free(sys_stats.cores_cpu_percent);
        free(proc_stats);
    }

    cleanup_display(); // ncurses 종료
    return 0;
}