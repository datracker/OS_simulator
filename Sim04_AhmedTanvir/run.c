/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#include "run.h"
#include <unistd.h>
#include "output.c"

//resource counter
int hdctr = 0;
int printctr = 0;
int interrupt_flag = 0;
//int *hdctr = &hd;
//int *printctr = &printer;
int lock;
sem_t mutex;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

//returns microseconds for running a process n times, timereq for running each time
void runProcess(proc4run* p)//, int thread) 
{
    struct timeval refTime, currentTime;
    gettimeofday(&refTime, NULL);

    int i = 0;
    p->totaltime = 0;
    while(i < p->cycletime)
    {
        p->totaltime += p->runtime;
        //printf("%s process running..\n", p->name);
        i++;
    }

    gettimeofday(&currentTime, NULL);

    currentTime.tv_usec += p->totaltime;

    int sec =  currentTime.tv_sec - refTime.tv_sec;
    int microsec =  currentTime.tv_usec - refTime.tv_usec;
    p->timePassed = timeElapsed(sec, microsec);

    //printf("timerequired>>: %f\n", timePassed);

    //free(data); //Free data, save memory, code better, live longer
    //return timePassed;

}

void *run(void *param)
{
    struct timeval refTime, currentTime;
    gettimeofday(&refTime, NULL);
    //sleep(1);

    proc4run *datum = param;
    datum->totaltime = 0;

    char event[255];
    char *temp;
    if(!strcmp(datum->io, "I"))
    {
        temp = "input";
    }
    else
    {
        temp = "output";
    }
    //printf(">>>hd %d\n", hdctr);
    //printf(">>>printer %d\n", printctr);

    if(!strcmp(datum->name, "harddrive"))
    {
        //sem_init(&mutex, 1, 1);
        while(hdctr >= datum->total_res);//printf("waiting...\n"); //spinlock
        pthread_mutex_lock( &mutex1 );
        hdctr++;
        //hdctr %= datum->total_res;
        datum->res_num = hdctr;
        //printf("BEFORE hd %d\n", *hdctr);
        pthread_mutex_unlock( &mutex1 );
    }
    else if(!strcmp(datum->name, "printer"))
    {
        //sem_init(&mutex, 1, 1);
        while(printctr >= datum->total_res);//printf("waiting...\n"); //spinlock
        pthread_mutex_lock( &mutex2 );
        printctr++;
        //printctr %= datum->total_res;
        datum->res_num = printctr;
        //printf("BEFORE printer %d\n", *printctr);
        pthread_mutex_unlock( &mutex2 );
    }
    else
    {
        //printf("I/O operation did not recognize: %s\n", datum->name);
    }

    pthread_mutex_lock( &mutex3 );

    if ( (!strcmp(datum->name, "harddrive")) || (!strcmp(datum->name, "printer")) )
    {
        //printf("*****************************************\n");
        sprintf(event, "%f - Process %d: start %s %s on %s %d\n", datum->prev_time, datum->app_num + 1, datum->name, temp, datum->name, datum->res_num); 
    }
    else
    {
        sprintf(event, "%f - Process %d: start %s %s\n", datum->prev_time, datum->app_num + 1, datum->name, temp); 
    }
    
    strcat(datum->logger, event);
    //printf("%f - Process %d: start %s %s on %s %d -- %u\n", datum->prev_time, datum->app_num, datum->name, temp, datum->name, datum->res_num, datum->total_res); 
    pthread_mutex_unlock( &mutex3 );

    int i = 0;
    
    while(i < datum->cycletime)
    {
        datum->totaltime += datum->runtime;
        i++;
        //sleep(1);
    }

    gettimeofday(&currentTime, NULL);
    currentTime.tv_usec += datum->totaltime;

    int sec =  currentTime.tv_sec - refTime.tv_sec;
    int microsec =  currentTime.tv_usec - refTime.tv_usec;
    datum->timePassed = timeElapsed(sec, microsec);
    datum->prev_time += datum->timePassed;
    //printf(">>>%f\n", datum->timePassed);

    pthread_mutex_lock( &mutex3 );
    if ( (!strcmp(datum->name, "harddrive")) || (!strcmp(datum->name, "printer")))
    {
        sprintf(event, "%f - Process %d: end %s %s on %s %d\n", datum->prev_time, datum->app_num + 1, datum->name, temp, datum->name, datum->res_num); 
    }
    else
    {
        sprintf(event, "%f - Process %d: end %s %s\n", datum->prev_time, datum->app_num + 1, datum->name, temp); 
    }
    //printf("%f - Process %d: end %s %s on %d\n", datum->prev_time, datum->app_num, datum->name, temp, datum->res_num);
    strcat(datum->logger, event);
    pthread_mutex_unlock( &mutex3 );

    
    //printf("BEFORE printer %d\n", printctr);

    if(!strcmp(datum->name, "harddrive"))
    {
        pthread_mutex_lock( &mutex1 );
        hdctr--;
        //printf("AFTER hd %d\n", *hdctr);
        pthread_mutex_unlock( &mutex1 );
    }
    else if(!strcmp(datum->name, "printer"))
    {
        pthread_mutex_lock( &mutex2 );
        printctr--;
        //printf("AFTER printer %d\n", *printctr);
        pthread_mutex_unlock( &mutex2 );
    }

    //sem_post(&mutex);

    pthread_exit(0);
    interrupt_flag++;
}


double timeElapsed(int seconds, int microseconds)
{
  if(microseconds < 0 && seconds > 0)
    {
       microseconds += 1000000;
       seconds -= 1;
       return  (double) microseconds/1000000 + (double) seconds;

    }

    else if(seconds == 0)
    {
        return (double) microseconds/1000000;
    }

    else if(seconds > 0 && microseconds > 0)
    {
    return  (double) microseconds/1000000 + (double) seconds;
    }
    else
    {
      return -1;
    }

}

unsigned int allocateMem(proc4run *p, unsigned int last_addr, unsigned int totalMem)
{
    /*
    adding to the last address, number of iteration multiplied by memory block size, returns a decimal value
    p->num_it is the number of iteration for each process
    Need to convert the decimal to hexa
    NOTE: addition is performing in decimal system
    */
    last_addr += p->cycletime * memBlockSize; 
    if (last_addr > totalMem)
    {
        perror("System out of memory, Allocating from the beginning: ");
        last_addr = 0;
        last_addr += p->cycletime * memBlockSize;
        return last_addr;
    }
    //printf("0x%08x\n", last_addr);
    return last_addr;
}