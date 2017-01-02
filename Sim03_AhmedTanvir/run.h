/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#ifndef TIMER_H_
#define TIMER_H_


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define memBlockSize 128   //defining the 128 kbytes block size for memory

typedef struct
{
	unsigned int cycletime, runtime, totaltime;
} proc4run;

double timeElapsed(int seconds, int microseconds);
unsigned long memAddress(unsigned long available);
double runProcess(procs p, int timereq, int thread);
void *run(void *param);
unsigned int allocateMem(procs p, unsigned int timereq, unsigned int totalMem);

#endif  /*TIMER_H_*/