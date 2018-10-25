#ifndef MYINTS_FILE
#define MYINTS_FILE
#include <kernel.h>

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72

extern volatile unsigned char scan;
extern volatile unsigned char ascii;
extern volatile unsigned char timer;
extern volatile unsigned int map_move_digger_pid, digger_move_pid, map_debug_pid, terminate_xinu_pid;

void set_new_int9_newisr();
extern void interrupt (*Int9Save) (void);
extern void interrupt (*Int8Save) (void);
INTPROC MyISR9(int mdevno);
void interrupt MyISR8(void);


#endif