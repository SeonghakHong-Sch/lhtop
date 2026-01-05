#pragma once

#include <ncurses.h>
#include "stats.h"
#include "process.h"
#include "system.h"

// ncurses initialization, cleanup
void init_display();
void cleanup_display();

// render entire screen
void render_display(SystemStats* sys_stats, SystemInfo* sys_info,
                   ProcessStats* proc_stats, ProcessList* proc_list);

// render individual sections
void render_header(SystemStats* sys_stats, SystemInfo* sys_info);
void render_process_table(ProcessStats* proc_stats, ProcessList* proc_list, SystemStats* sys_stats, int start_row);

// utility function
void draw_bar(int y, int x, float percent, int width);
