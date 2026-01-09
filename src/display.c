#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "../include/display.h"

// ncurses initialization: switch terminal to ncurses mode
void init_display() {
    initscr();              // start ncurses
    cbreak();               // disable line buffering (process input immediately)
    noecho();               // don't display typed characters
    curs_set(0);            // hide cursor
    keypad(stdscr, TRUE);   // enable special keys (arrow keys, etc)
    nodelay(stdscr, TRUE);  // set getch() to non-blocking

    // enable colors (optional)
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);   // CPU bar
        init_pair(2, COLOR_CYAN, COLOR_BLACK);    // memory bar
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // header
    }
}

// ncurses cleanup: restore terminal to original state
void cleanup_display() {
    endwin();  // end ncurses, restore terminal
}

// draw progress bar
// y, x: start position, percent: 0.0~1.0, width: bar width
void draw_bar(int y, int x, float percent, int width) {
    int filled = (int)(percent * width);  // filled portion length

    mvaddch(y, x, '[');  // start bracket
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            addch('|');  // filled portion
        } else {
            addch(' ');  // empty portion
        }
    }
    addch(']');  // end bracket
}

// render header: system-wide CPU, memory, swap info
void render_header(SystemStats* sys_stats, SystemInfo* sys_info) {
    int row = 0;

    // title
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(row++, 0, "lhtop - Linux System Monitor");
    attroff(COLOR_PAIR(3) | A_BOLD);
    row++;

    // total CPU usage
    attron(COLOR_PAIR(1));
    mvprintw(row, 0, "Total CPU  [%5.1f%%] ", sys_stats->total_cpu_percent * 100);
    attroff(COLOR_PAIR(1));
    draw_bar(row, 21, sys_stats->total_cpu_percent, 30);
    row++;

    // per-core CPU usage (display 4 cores per line)
    int cores = sys_info->cores;
    int cols_per_line = 4;  // 4 cores per line

    for (int i = 0; i < cores; i++) {
        if (i % cols_per_line == 0 && i != 0) row++;  // newline every 4 cores

        int col = (i % cols_per_line) * 20;  // each core takes 20 columns
        mvprintw(row, col, "core %2d[%5.1f%%]", i, sys_stats->cores_cpu_percent[i] * 100);
        //draw_bar(row, col + 10, sys_stats->cores_cpu_percent[i], 8);
    }
    row += 2;

    // memory usage
    attron(COLOR_PAIR(2));
    mvprintw(row, 0, "Mem  [%5.1f%%] ", sys_stats->mem_percent * 100);
    attroff(COLOR_PAIR(2));
    draw_bar(row, 16, sys_stats->mem_percent, 30);

    // memory capacity (KB -> GB)
    float used_gb = (sys_info->total_mem - sys_info->avail_mem) / 1024.0 / 1024.0;
    float total_gb = sys_info->total_mem / 1024.0 / 1024.0;
    mvprintw(row, 50, "%.1fG / %.1fG", used_gb, total_gb);
    row++;

    // swap usage
    attron(COLOR_PAIR(2));
    mvprintw(row, 0, "Swap [%5.1f%%] ", sys_stats->swap_percent * 100);
    attroff(COLOR_PAIR(2));
    draw_bar(row, 16, sys_stats->swap_percent, 30);

    float swap_used_gb = (sys_info->swap_total - sys_info->swap_free) / 1024.0 / 1024.0;
    float swap_total_gb = sys_info->swap_total / 1024.0 / 1024.0;
    mvprintw(row, 50, "%.1fG / %.1fG", swap_used_gb, swap_total_gb);
    row++;
}

// render process table: display PID, name, CPU%, MEM%
void render_process_table(ProcessStats* proc_stats, ProcessList* proc_list, SystemStats* sys_stats, int start_row) {
    int row = start_row;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);  // get terminal size

    float cpu_percent = sys_stats->total_cpu_percent * 100;
    float mem_percent = sys_stats->mem_percent * 100;

    // table header
    attron(A_REVERSE);  // reverse video (swap foreground/background)
    mvprintw(row++, 0, "  PID     CPU    MEM   STATE   COMMAND");
    mvprintw(row++, 0, "TOTAL  %5.1f%%  %5.1f%%                 ", cpu_percent, mem_percent);
    attroff(A_REVERSE);

    // create index array for sorting by CPU usage
    int* sorted_indices = (int*)malloc(sizeof(int) * proc_list->count);
    for (int i = 0; i < proc_list->count; i++) {
        sorted_indices[i] = i;
    }

    // simple bubble sort (CPU descending)
    for (int i = 0; i < proc_list->count - 1; i++) {
        for (int j = 0; j < proc_list->count - i - 1; j++) {
            if (proc_stats[sorted_indices[j]].cpu_percent <
                proc_stats[sorted_indices[j + 1]].cpu_percent) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }

    // display processes that fit on screen
    int display_count = max_y - row - 2;  // bottom margin 2 lines
    if (display_count > proc_list->count) {
        display_count = proc_list->count;
    }

    for (int i = 0; i < display_count && row < max_y - 1; i++) {
        int idx = sorted_indices[i];
        ProcessInfo* proc = &proc_list->processes[idx];
        ProcessStats* stat = &proc_stats[idx];

        // highlight high CPU usage
        if (stat->cpu_percent > 0.5) {  // over 50%
            attron(A_BOLD | COLOR_PAIR(1));
        }

        mvprintw(row++, 0, "%6d  %5.1f  %5.1f     %c     %s",
                 proc->pid,
                 stat->cpu_percent * 100,
                 stat->mem_percent * 100,
                 proc->state,
                 proc->comm);

        if (stat->cpu_percent > 0.5) {
            attroff(A_BOLD | COLOR_PAIR(1));
        }
    }

    free(sorted_indices);

    // bottom status bar
    attron(A_REVERSE);
    mvprintw(max_y - 1, 0, "Total: %d processes | Press 'q' to quit", proc_list->count);
    attroff(A_REVERSE);
}

// render entire display: header + process table
void render_display(SystemStats* sys_stats, SystemInfo* sys_info,
                   ProcessStats* proc_stats, ProcessList* proc_list) {
    clear();  // clear screen

    // render header section (top ~10 lines)
    render_header(sys_stats, sys_info);

    // render process table (below header)
    render_process_table(proc_stats, proc_list, sys_stats, 10);

    refresh();  // output to screen (double buffering)
}
