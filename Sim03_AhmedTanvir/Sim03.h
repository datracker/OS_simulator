/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#ifndef SIM03_H_
#define SIM03_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include "file_read.c"
#include "read_mdf.c"
#include "run.c"
#include "output.c"

#define _GNU_SOURCE
#define threadTrue 1
#define threadFalse 0
#define latency .000001


typedef struct
{
	char process_stat [15]; //status of the process, new, waiting, running etc.
	int pid;
}PCB;

int findTotalProcess(char *file)
{
	FILE *fp;
    fp = fopen(file, "r");

	if(fp == NULL)
	{   
		perror("Error in opening Meta-data file: ");
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


#endif  /*SIM03_H_*/