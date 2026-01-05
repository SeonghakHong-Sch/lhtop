#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>

#define BUF_SIZE 256
#define CUSTOM_CLK 1 //sec 기준

#define INITIAL_CAPACITY 256
#define MAX_CAPACITY 4096

#define JIFFIES sysconf(_SC_CLK_TCK) //1초 몇 틱이냐
#define PAGE_SIZE (sysconf(_SC_PAGESIZE) / 1024) //페이지 사이즈, KB로 치환

typedef unsigned long TIME;

#endif