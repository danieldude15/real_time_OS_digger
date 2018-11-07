#ifndef MYINTS_FILE
#define MYINTS_FILE
#include <dos.h>
#include <kernel.h>
#include <stdio.h>
#include <conf.h>
#include <io.h>
#include <sleep.h>
#include <proc.h>
#include <butler.h>

#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define UP_ARROW 72
#define SPACE_BAR 57


#define SECONDT 1000
#define LETCH 900
#define FACTOR 2
#define DELTA_SPEED 0.2
#define DIGGER_SPEED 250
#define MONSTER_80 300
#define MONSTER_120 200

extern int num_of_pids,dig_uppid,mon_uppid, dispid, nob_uppid,recvpid, debug,sound_effects_pid,score_lives_pid,terminate_xinu_pid,nobbin_creator_pid,receiver_pid;
extern long tod;
extern int num_of_pids,pressed_flag,pressed;

extern int sched_arr_pid[10];
extern int sched_arr_int[10];
extern int point_in_cycle;
extern int gcycle_length;
extern int gno_of_pids;


void setup_interrupts();
void restore_ints();
void kill_xinu();
extern INTPROC (*Int9Save)(int);
INTPROC MyISR9(int mdevno);

#endif