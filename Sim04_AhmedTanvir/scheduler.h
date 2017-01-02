/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_



#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int total_cycle, stpoint, endpoint, total_proc;
}sched_info;

void scheduler(procs *process, int* list, int size, int schedulerName, int totalProc);
void flashQueue(int** list, int size1, int size2);
int checkQueue(int* list, int size);
void printValue(apps* app, int size);
//void sort(sched_info *sched_list, int size);
void addcycles(sched_info *sched_list, procs *process, apps *app, int totalProc, int totalapp);
int countApp(procs *process, int totalProc);
void makelist(sched_info *sched_list, int** list, int totalProc, int total_app);
void printList(int** list, int size1, int size2);
void load_apps(apps *app, procs *new, int totalapp, int totalProc);
int totalCycle(apps* app, int total_app);
void setValue(int *list, int size);
void countcycles(sched_info *sched_list, proc4run *process, int totalProc);
void flashData(proc4run *proc);
int next_app(char *schduler, apps *app, int present_app, int total_app);
void sort(apps *app, int size);


#endif  /*TIMER_H_*/