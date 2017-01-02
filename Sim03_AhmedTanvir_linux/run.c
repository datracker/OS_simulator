/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#include "run.h"


double runProcess(procs p, int timereq, int thread) //returns microseconds for running a process n times, timereq for running each time
{
    /*Below will run actually using pthread, because pthread uses a function 
    which cannot take more than one argument, that's why made a struct to 
    incorporate multiple arguments. data is an instance of proc4run struct
    */
    proc4run *data = malloc(sizeof(proc4run)); 
    data->cycletime = p.num_it;
    //printf("%s\n", p.proctag);
    //printf(">>>>%d\n", data->cycletime);
    data->runtime = timereq;
    //printf("%d\n", data->runtime);

    struct timeval refTime, currentTime;
    gettimeofday(&refTime, NULL);

    if(thread > 0)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, run, data);
        pthread_join(tid, NULL);   
        //printf("Pthread: %d\n", data->totaltime);
    }
    else
    {
        int i = 0;
        data->totaltime = 0;
        while(i < data->cycletime)
        {
            data->totaltime += data->runtime;
            i++;
        }
        //printf("Unthread: %d\n", data->totaltime);
    }

    gettimeofday(&currentTime, NULL);

    currentTime.tv_usec += data->totaltime;

    int sec =  currentTime.tv_sec - refTime.tv_sec;
    int microsec =  currentTime.tv_usec - refTime.tv_usec;
    double timePassed = timeElapsed(sec, microsec);

    //printf("timerequired>>: %f\n", timePassed);

    free(data); //Free data, save memory, code better, live longer
    return timePassed;

}

void *run(void *param)
{
    int i = 0;
    proc4run *datum = param;
    datum->totaltime = 0;
    while(i < datum->cycletime)
    {
        datum->totaltime += datum->runtime;
        i++;
    }
    pthread_exit(0);
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

unsigned int allocateMem(procs p, unsigned int last_addr, unsigned int totalMem)
{
    /*
    adding to the last address, number of iteration multiplied by memory block size, returns a decimal value
    p->num_it is the number of iteration for each process
    Need to convert the decimal to hexa
    NOTE: addition is performing in decimal system
    */
    last_addr += p.num_it * memBlockSize; 
    if (last_addr > totalMem)
    {
        perror("System out of memory, Allocating from the beginning: ");
        last_addr = 0;
        last_addr += p.num_it * memBlockSize;
        return last_addr;
    }
    //printf("0x%08x\n", last_addr);
    return last_addr;
}




