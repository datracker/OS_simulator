/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#include "scheduler.h"

void load_apps(apps *app, procs *new, int totalapp, int totalProc)
{
    int start = 0;
    for (int i = 0; i <= totalapp; ++i)
    {
        for (int j = start; j < totalProc; ++j)
        {
            if(!strcmp(new[i].proctag, "A"))
            {
                strcpy(app[i].process[j].proctag, new[j].proctag);
                strcpy(app[i].process[j].procjob, new[j].procjob);
                app[i].process[j].num_it = new[j].num_it;
            }
        }
        
    }
}


void scheduler(procs *process, int* list, int size, int schedulerName, int totalProc)
{
    //printf("%s\n", process[2].proctag);
    //flashQueue(list, size);
    int total_app = countApp(process, totalProc);
    sched_info *sched_list = malloc(sizeof(sched_info) * total_app);
    //addcycles(sched_list, process, totalProc);  
    //sort(sched_list, total_app);

    //makelist(sched_list, list, totalProc, total_app);
    //printList(list, totalProc - 1);
    //printValue(sched_list, total_app);


    if(schedulerName == 1)
    {
        //SJF();
    }
    free(sched_list);
}

void makelist(sched_info *sched_list, int** list, int totalProc, int total_app)
{
    flashQueue(list, total_app, totalProc);
    //list[0] = '\0'; //NULL
    list[0][0] = 1; //reserved for S(start)
    int ctr = 0;

    for (int i = 0; i < total_app; ++i)
    {
        for (int j = sched_list[i].stpoint; j <= sched_list[i].endpoint; ++j)
        {
            list[i + 1][ctr] = j;
            //printf("%d -- %d\n", j, ctr);
            ctr++;      
        }
        ctr = 0;
    }
    //printf("%d -- %d\n", totalProc, ctr);
    list[total_app + 1][0] = totalProc; // reserved for S(end)
}

int countApp(procs *process, int totalProc)
{
    int ctr = 0;

    for (int i = 1; i < totalProc; ++i)
    {
        if( (!strcmp(process[i].proctag, "A")) && (!strcmp(process[i].procjob, "start")) )
        {
            ctr++;
        }
    }
    return ctr;
}

//Add the number of cycles to in each application
void addcycles(sched_info *sched_list, procs *process, apps *app, int totalProc, int total_app)
{
    //setValue(sched_list, 20);
    int ctr = 0;
    int flag1, flag2;
    int sum = 0;

    for (int i = 1; i < totalProc; ++i)
    {
        if( (!strcmp(process[i].proctag, "A")) && (!strcmp(process[i].procjob, "start")) )
        {
            flag1 = i;
            //printf(">>%d  %s -- %s\n", flag1, process[i].proctag, process[i].procjob);
        }

        if( (!strcmp(process[i].proctag, "A")) && (!strcmp(process[i].procjob, "end")) )
        {
            flag2 = i;
            //printf("**%d  %s -- %s\n", flag2, process[i].proctag, process[i].procjob);
            for (int i = flag1; i < flag2; ++i)
            {
                sum += process[i].num_it;
                //printf("%s\n", process[i].proctag);
            }
            //printf("sum: %d\n", sum);
            sched_list[ctr].total_cycle = sum;
            sched_list[ctr].stpoint = flag1;
            sched_list[ctr].endpoint = flag2;
            sched_list[ctr].total_proc = flag2 - flag1;
            ctr++;
            sum = 0;
        }
    }

    for (int i = 0; i < total_app; ++i)
    {
        app[i].cycle_remain = sched_list[i].total_cycle;
        app[i].is_finished = 1;
    }

}


void sort(apps *app, int size)
{
    int i, j;
    apps temp;

    for (i = 1; i < size; i++)
    {
        for (j = 0; j < size - i; j++) 
        {
            if (app[j].cycle_remain > app[j+1].cycle_remain) {
                temp = app[j];
                app[j] = app[j + 1];
                app[j + 1] = temp;
            }
        }
    }
}


void flashQueue(int** list, int size1, int size2)
{
    for (int i = 0; i < size1 - 1; ++i)
    {
        for (int j = 0; j < size2; ++j)
        {
            list[i][j] = '\0';
        }
    }
}
/*
int checkQueue(int* list, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if(list[i] != '\0')
        {
            return 1; //means there is at least one process to be run
        }
    }
    return 0;
}
*/

void printValue(apps *app, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("---_%d___%d_---\n", app[i].cycle_remain, app[i].is_finished);
    }
}

void printList(int** list, int size1, int size2)
{
    for (int i = 0; i < size1 - 1; ++i)
    {
        for (int j = 0; j < size2; ++j)
        {
            printf("%d\n", list[i][j]);;
        }
    }
}

int totalCycle(apps* app, int size)
{
    int sum = 0;
    for (int i = 0; i < size; ++i)
    {
        sum += app[i].cycle_remain;
    }
    return sum;
}

int appLeft(apps* app, int size)
{
    int sum = 0;
    for (int i = 0; i < size; ++i)
    {
        sum += app[i].is_finished;
    }
    return sum;
}

void setValue(int* list, int size)
{
    for (int i = 0; i < size; ++i)
    {
        list[i] = 0;
    }
}

void countcycles(sched_info *sched_list, proc4run *process, int totalProc)
{
    //setValue(sched_list, 20);
    int ctr = 0;
    int flag1, flag2;
    int sum = 0;

    for (int i = 1; i < totalProc; ++i)
    {
        if( (!strcmp(process[i].io, "A")) && (!strcmp(process[i].name, "start")) )
        {
            flag1 = i;
            //printf(">>%d  %s -- %s\n", flag1, process[i].proctag, process[i].procjob);
        }

        if( (!strcmp(process[i].io, "A")) && (!strcmp(process[i].name, "end")) )
        {
            flag2 = i;
            //printf("**%d  %s -- %s\n", flag2, process[i].proctag, process[i].procjob);
            for (int i = flag1; i < flag2; ++i)
            {
                sum += process[i].cycletime;
                //printf("%s\n", process[i].proctag);
            }
            //printf("sum: %d\n", sum);
            sched_list[ctr].total_cycle = sum;
            sched_list[ctr].stpoint = flag1;
            sched_list[ctr].endpoint = flag2;
            sched_list[ctr].total_proc = flag2 - flag1;
            ctr++;
            sum = 0;
        }
    }

}

void flashData(proc4run *proc)
{
    /*
    unsigned int cycletime, runtime, totaltime, app_num, res_num, total_res;
    double timePassed, prev_time;
    char *logger;
    char name[20], io[5];
    unsigned int cycle_left, will_run;
    */

    proc->cycletime = 0;
    proc->runtime = 0;
    proc->totaltime = 0;
    proc->app_num = 0;
    proc->res_num = 0;
    proc->total_res = 0;
    //proc->cycle_left = 0;

    proc->timePassed = 0;
    proc->prev_time = 0;

    //strcpy(proc->logger, '\0');
    //strcpy(proc->name, '\0');
    //strncpy(proc->io, '\0', sizeof(proc->io));
}

int next_app(char *schduler, apps *app, int present_app, int total_app)
{
    if ( (!strcmp(schduler, "RR")) || (!strcmp(schduler, "SJF")) )
    {
        present_app++;
        present_app %= total_app;

        for (int i = 0; i < total_app; ++i)
        {
            if (app[present_app].is_finished > 0)
            {
                //printf("**************************%d\n", present_app); //works like a boss
                return present_app;
            }
            else
            {
                //printf("############################\n"); //works
                present_app++;
                present_app %= total_app;
            }
        }
        return -1; //done running all the apps
    }

    else if ( (!strcmp(schduler, "SRTF")) || (!strcmp(schduler, "STRF")) )
    {
        sort(app, total_app);

        for (int i = 0; i < total_app; ++i)
        {
            if (app[i].is_finished > 0)
            {
                //printf("**************************%d\n", present_app); //works like a boss
                return i;
            }
        }
        return -1; //done running all the apps
    }
    else
    {
        perror("Error reading scheduler type: ");
        return -2;
    }
    
}



