/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#ifndef SIM04_H_
#define SIM04_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include "file_read.c"
#include "read_mdf.c"
#include "run.c"
#include "output.c"
#include "scheduler.c"

#define _GNU_SOURCE
//#define threadTrue 1
//#define threadFalse 0
#define latency .000001
#define RR 1
#define SJF 2
#define STRF 3


/*typedef struct
{
	char process_stat [15]; //status of the process, new, waiting, running etc.
	int pid;
}PCB;*/

/*typedef struct
{
    unsigned int cycletime, runtime, totaltime;
    double timePassed;
}proc4run;*/


int findTotalProcess(char *file)
{
	FILE *fp;
    fp = fopen(file, "r");

	if(fp == NULL)
	{   
		perror("Cannot find total processes, error in opening Meta-data file: ");
    }

    int totalProc = 0;
    do
    {
        int c = fgetc(fp);
		if( feof(fp) )
        {
            break ;
        }
        else if(c == 59) //ASCII value of semicolon
        {
            totalProc++;
		}
        //printf("%c", c);
    }while(1);
        //printf("%d\n", totalProc);
    fseek(fp, 0L, SEEK_SET); //puts filepointer again at the beginning of the file
    fclose(fp); 

    return totalProc;
}


#endif  /*SIM04_H_*/