/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#include "Sim04.h"


int main(int argc, char const *argv[])
{
    
    //sem_init(&mutex, 1, 1);

	if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename", argv[0] );
    }
    else 
    {
        //reading the config file and storing data in heap using the Infoconf struct
        InfoConf *config = malloc(sizeof(InfoConf));
        read_config(argv[1], config);
        //printf("***%s\n", config->scode);


        int totalProc = findTotalProcess(config->file_path);
        totalProc ++; //This counts the last process, which doesnot end in semicolon

        //logs for every event, an event is the a single line of output
        //logs store all data temporarily in heap, will be written in disk or monitor later
        char event[255];
        char* logs = malloc(sizeof(event)*(totalProc*3));
        

        //reading the meta-data file and storing data in heap using the procs struct
        procs *new = malloc(sizeof(procs) * totalProc);
        read_mdf(config->file_path, new); //all proceses loaded into new queue


        double init_time = 0.000001; //starting time of the program
        unsigned int address = 0x00000000; //starting address of the memory location in HEX
        //printf("0x%08x\n", address);


        int totalapp = countApp(new, totalProc); // Finding total number of apps
        //printf("%d -- %d\n", totalapp, totalProc);

        //Creating the ready_queue for all the apps
        apps *ready_queue = malloc(totalapp * sizeof(apps));
        for (int i = 0; i < totalapp; ++i)
        {
            ready_queue[i].process = malloc(totalProc * sizeof(procs));
            ready_queue[i].cycle_remain = 0;
        }

        //loading all the apps to the ready_queue from new except S(start) and S(end) which are not app
        load_app(ready_queue, new, totalProc); 

        //helps the set up the scheduler
        sched_info *sched_list = malloc(sizeof(sched_info) * totalapp);
        //Add apps cycle information in the ready_queue
        addcycles(sched_list, new, ready_queue, totalProc, totalapp);

        //printf("Before Sorting: \n");
        //printValue(ready_queue, totalapp); //ok
        if ( (!strcmp(config->scode, "SJF")) || (!strcmp(config->scode, "STRF")) || (!strcmp(config->scode, "SRTF")))
        {
            sort(ready_queue, totalapp);
        }
        //printf("After Sorting:\n");
        //printValue(ready_queue, totalapp); //ok


        /*multithreading initialization. Every thread will be working independently irrespective of the 
        progress of other processes/threads. That's why joining was written after the while loop. 
        The while loop will go through the jobs in every process, and execute inside of a thread 
        when it finds any I/O.
        */
        pthread_t tid[totalProc];
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        int threadctr = 0;
        int anothercounter = 0; //counter for the specialized struct of process for run

        proc4run *data = malloc(sizeof(proc4run) * totalProc); 
        int app_ctr = 0, proc_ctr = 0;
        int app_remain = appLeft(ready_queue, totalapp);
        int quantum_time = config->qnum;
        int last_proc[totalapp]; //remembers the last job executed
        setValue(last_proc, totalapp); //initilize with zeros

        //new[0] has garbage value, starting from 1
        data[anothercounter].cycletime = new[proc_ctr + 1].num_it;
        data[anothercounter].prev_time = init_time;
        data[anothercounter].logger = logs;
        data[anothercounter].app_num = app_ctr;
        data[anothercounter].res_num = 0;
        strcpy(data[anothercounter].name, new[proc_ctr + 1].procjob);
        strcpy(data[anothercounter].io, new[proc_ctr + 1].proctag);

        if((!strcmp(data[anothercounter].io, "S")) && (!strcmp(data[anothercounter].name, "start")) )
        {
            data[anothercounter].runtime = 0;
            runProcess(&data[anothercounter]);
            init_time += data[anothercounter].timePassed;
            pthread_mutex_lock( &mutex3 );
            sprintf(event, "%f - Simulator program starting\n", init_time);
            strcat(logs, event);
            pthread_mutex_unlock( &mutex3 );
        }

        while(app_remain > 0 ) //doesn't include last two processes (A and S)
        {
            if (interrupt_flag > 0)
            {
                for (int i = 0; i < threadctr; ++i)
                {
                    pthread_join(tid[i], NULL);   
                }
                interrupt_flag = 0;
            }
               
            //make a signal for checking if there's I/O
            app_ctr %= totalapp;
            anothercounter %= (totalProc - 2); //ok 
            anothercounter++; //correct


            data[anothercounter].cycletime = ready_queue[app_ctr].process[last_proc[app_ctr]].num_it;
            data[anothercounter].prev_time = init_time;
            data[anothercounter].logger = logs;
            data[anothercounter].app_num = app_ctr;
            data[anothercounter].res_num = 0;
            strcpy(data[anothercounter].name, ready_queue[app_ctr].process[last_proc[app_ctr]].procjob);
            strcpy(data[anothercounter].io, ready_queue[app_ctr].process[last_proc[app_ctr]].proctag);

            //Where there's an I/O there's a thread
            
            if( (!strcmp(data[anothercounter].io, "I")) || (!strcmp(data[anothercounter].io, "O")) )
            {
                assert(!strcmp(ready_queue[app_ctr].process[last_proc[app_ctr]].proctag, data[anothercounter].io));

                if( (!strcmp(data[anothercounter].name, "printer")) && data[anothercounter].cycletime > 0)
                {
                    data[anothercounter].total_res = config->prinquant;
                    data[anothercounter].runtime = config->printime;
                    pthread_create(&tid[threadctr], &attr, run, &data[anothercounter]);
                    threadctr++;
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it = 0;
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    quantum_time = config->qnum; //recharging quantum
                    //sched_list[app_ctr].total_cycle -= data->cycletime;
                    //printf("Allcycles printer: %d   %d\n", allCycle, sched_list[app_ctr].total_cycle);
                    last_proc[app_ctr]++;
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                }
                

                else if( (!strcmp(data[anothercounter].name, "keyboard")) && data[anothercounter].cycletime > 0 )
                {
                    data[anothercounter].total_res = 1;
                    data[anothercounter].runtime = config->keytime;
                    pthread_create(&tid[threadctr], &attr, run, &data[anothercounter]);
                    threadctr++;
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it = 0;
                    quantum_time = config->qnum;
                    //printf("Allcycles harddrive: %d   %d\n", allCycle, sched_list[app_ctr].total_cycle);
                    last_proc[app_ctr]++;
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                }

                else if( (!strcmp(data[anothercounter].name, "monitor")) && data[anothercounter].cycletime > 0 )
                {
                    data[anothercounter].total_res = 1;
                    data[anothercounter].runtime = config->montime;
                    pthread_create(&tid[threadctr], &attr, run, &data[anothercounter]);
                    threadctr++;
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it = 0;
                    last_proc[app_ctr]++;
                    quantum_time = config->qnum;
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                }
                

                else if( (!strcmp(data[anothercounter].name, "harddrive")) && data[anothercounter].cycletime > 0 )
                {
                    data[anothercounter].total_res = config->hdquant;
                    data[anothercounter].runtime = config->hardtime;
                    pthread_create(&tid[threadctr], &attr, run, &data[anothercounter]);
                    threadctr++; 
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it = 0;
                    last_proc[app_ctr]++;
                    quantum_time = config->qnum;
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                }
               
            }
            
            
            if((!strcmp(data[anothercounter].io, "P")) && data[anothercounter].cycletime > 0)
            {
                assert(!strcmp(ready_queue[app_ctr].process[last_proc[app_ctr]].proctag, data[anothercounter].io));
                //printf("%d--%d\n", data[anothercounter].cycletime, quantum_time); ok
                int temp_app_number;
                if (data[anothercounter].cycletime > quantum_time)
                {
                    data[anothercounter].cycletime = quantum_time;
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it -= quantum_time;
                    //printf("****%d\n",ready_queue[app_ctr].process[last_proc[app_ctr]].num_it); ok
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    ///
                    data[anothercounter].runtime = config->proctime;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: start processing action\n", init_time, app_ctr + 1);
                    strcat(logs, event);
                    pthread_mutex_unlock( &mutex3 );
                    runProcess(&data[anothercounter]); 
                    init_time += data[anothercounter].timePassed; 
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: interrupt processing action\n", init_time, app_ctr + 1);
                    strcat(logs, event);  
                    pthread_mutex_unlock( &mutex3 );
                    ///

                    //temp_app_number = app_ctr;
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                    quantum_time = config->qnum; //recharging quantum time
                }
                else
                {
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it = 0; //will work full cycles
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    ///
                    data[anothercounter].runtime = config->proctime;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: start processing action\n", init_time, temp_app_number + 1);
                    strcat(logs, event);
                    pthread_mutex_unlock( &mutex3 );
                    runProcess(&data[anothercounter]); 
                    init_time += data[anothercounter].timePassed; 
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: end processing action\n", init_time, temp_app_number + 1);
                    strcat(logs, event);  
                    pthread_mutex_unlock( &mutex3 );
                    ///

                    if (data[anothercounter].cycletime == quantum_time)
                    {
                        temp_app_number = app_ctr;
                        app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                        quantum_time = config->qnum; //recharge qtime
                    }
                    else
                    {
                        last_proc[app_ctr]++;
                        quantum_time -= data[anothercounter].cycletime; //rest of the quantum time
                    }                    
                }
            }
            
            /*
            Will M(allocate) and M(cache) will also be interrupted? I have implemented them as they are interruptible.
            But, I decided not to show the interruption in logfile. The Assignemnt didnot describe that clearly.
            Please, let me know if you want to add that feature, it's perfectly doable.
            */
            else if((!strcmp(data[anothercounter].io, "M")) && data[anothercounter].cycletime > 0)
            {
                assert(!strcmp(ready_queue[app_ctr].process[last_proc[app_ctr]].proctag, data[anothercounter].io));
                int temp_app_number;

                //M(cache operation) //works great
                //printf("%d ******* %d\n", data[anothercounter].cycletime, quantum_time);   
                if(data[anothercounter].cycletime < quantum_time && (!strcmp(data[anothercounter].name, "cache")))
                {
                    //printf("%d ++++++ %d\n", data[anothercounter].cycletime, quantum_time);   
                    int offset = 0, sum = 0;                    
                    while(sum < quantum_time)   //sum = total cycles to be cached
                    {
                        sum += ready_queue[app_ctr].process[last_proc[app_ctr] + offset].num_it;
                        offset++;
                    }
                    //printf("caching..............%d\n", offset);
                    for (int i = 1; i < offset; ++i)
                    {
                        //printf("%d...%d\n", app_ctr, last_proc[app_ctr] + i);
                        if (ready_queue[app_ctr].process[last_proc[app_ctr] + i].num_it >= 3)
                        {
                            ready_queue[app_ctr].process[last_proc[app_ctr] + i].num_it -= 2; 
                        }
                    }

                }
                

                if (data[anothercounter].cycletime > quantum_time)
                {
                    data[anothercounter].cycletime = quantum_time;
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it -= quantum_time;
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    temp_app_number = app_ctr;
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                    quantum_time = config->qnum; //recharge qtime
                }
                else
                {
                    ready_queue[app_ctr].process[last_proc[app_ctr]].num_it = 0;
                    ready_queue[app_ctr].cycle_remain -= data[anothercounter].cycletime;
                    if (data[anothercounter].cycletime == quantum_time)
                    {
                        temp_app_number = app_ctr;
                        app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                        quantum_time = config->qnum; //recharge qtime
                    }
                    else
                    {
                        last_proc[app_ctr]++;
                        quantum_time -= data[anothercounter].cycletime; //rest of the quantum time
                    }                         
                }

                if(!strcmp(data[anothercounter].name, "cache"))
                {
                    data[anothercounter].runtime = config->memtime;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: start memory caching\n", init_time, temp_app_number + 1);
                    strcat(logs, event);
                    pthread_mutex_unlock( &mutex3 );
                    runProcess(&data[anothercounter]);
                    init_time += data[anothercounter].timePassed;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: end memory caching\n", init_time, temp_app_number + 1);
                    strcat(logs, event);  
                    pthread_mutex_unlock( &mutex3 );
                }
                
                else if(!strcmp(data[anothercounter].name, "allocate") )
                {
                    data[anothercounter].runtime = config->memtime;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: allocating memory\n", init_time, temp_app_number + 1);
                    strcat(logs, event);
                    pthread_mutex_unlock( &mutex3 );
                    runProcess(&data[anothercounter]);
                    init_time += data[anothercounter].timePassed;
                    address = allocateMem(&data[anothercounter], address, config->sysmem);
                    //printf("0x%08x\n", address);
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - Process %d: memory allocated at 0x%08x\n", init_time, temp_app_number + 1, address);
                    strcat(logs, event);
                    pthread_mutex_unlock( &mutex3 );
                }
            }
            

            else if (!strcmp(data[anothercounter].io, "A") )
            {
                if(!strcmp(data[anothercounter].name, "start"))
                {
                    assert(!strcmp(ready_queue[app_ctr].process[last_proc[app_ctr]].proctag, data[anothercounter].io));
                    data[anothercounter].runtime = 0; //by default
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - OS: preparing process %d\n", init_time, app_ctr + 1);
                    strcat(logs, event);
                    pthread_mutex_unlock( &mutex3 );
                    runProcess(&data[anothercounter]);
                    init_time += data[anothercounter].timePassed;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - OS: starting process %d\n", init_time, app_ctr + 1);
                    strcat(logs, event);     
                    pthread_mutex_unlock( &mutex3 );
                    last_proc[app_ctr]++;
                }

                else if( (!strcmp(data[anothercounter].name, "end")) ) 
                {
                    while(interrupt_flag > 0);
                    assert(!strcmp(ready_queue[app_ctr].process[last_proc[app_ctr]].proctag, data[anothercounter].io));
                    ready_queue[app_ctr].is_finished = 0; //be careful
                    //printf(">>>>>>>>>>>>>>>>>>>>reached end %d<<<<<<<<<<<<<<<<<<\n", app_ctr);
                    //printValue(ready_queue, totalapp);
                    data[anothercounter].runtime = 0;
                    runProcess(&data[anothercounter]);
                    init_time += data[anothercounter].timePassed;
                    pthread_mutex_lock( &mutex3 );
                    sprintf(event, "%f - OS: process %d completed\n", init_time, app_ctr + 1);
                    strcat(logs, event);     
                    pthread_mutex_unlock( &mutex3 );
                    app_ctr = next_app(config->scode, ready_queue, app_ctr, totalapp); //go to next app
                }
                
            }

            else if (data[anothercounter].cycletime < 1)
            {
                last_proc[app_ctr]++;
            }

            else
            {
                pthread_mutex_lock( &mutex3 );
                //perror("Process not recognized: ");
                pthread_mutex_unlock( &mutex3 );
            }

            app_remain = appLeft(ready_queue, totalapp);

            //free(&data[anothercounter]);
           // sem_destroy(&mutex);
        }

        //while(app_remain > 0);
        data[totalProc].runtime = 0;
        runProcess(&data[totalProc]);
        init_time += data[anothercounter].timePassed;
        pthread_mutex_lock( &mutex3 );
        sprintf(event, "%f - Simulator program ending\n", init_time);
        strcat(logs, event);
        pthread_mutex_unlock( &mutex3 );


        
        //printf("%s\n", logs);
        //printf("%s ===== %s\n", config->log, config->log_path);
        output(logs, config->log, config->log_path);
        free(data);
        free(logs);
        free(new);
        free(config);
    }
	return 0;
}






