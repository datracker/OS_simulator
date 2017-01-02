/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#ifndef TIMER_H_
#define TIMER_H_


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define memBlockSize 128   //defining the 128 kbytes block size for memory
#define MAX_PROCESS 10000

typedef struct
{
	/*
	cycletime = number of iteration, runtime = time required for each iteration
	totaltime = total time passed so far, app_num = application number
	res_num = resource number, timePassed = time required for this process to run
	prev_time = time when the process started execution
	logger = event regarding each process logged here
	name = process name io= is it an I/O

	*/
	unsigned int cycletime, runtime, totaltime, app_num, res_num, total_res;
	double timePassed, prev_time;
	char *logger;
	char name[20], io[5];
} proc4run;

double timeElapsed(int seconds, int microseconds);
unsigned long memAddress(unsigned long available);
void runProcess(proc4run* p);
//void *run(void *param);
unsigned int allocateMem(proc4run *p, unsigned int timereq, unsigned int totalMem);

#endif  /*TIMER_H_*/