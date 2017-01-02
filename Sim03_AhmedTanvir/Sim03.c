/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#include "Sim03.h"


sem_t mutex;

int main(int argc, char const *argv[])
{
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
        //printf("***%u\n", config->sysmem);


        int totalProc = findTotalProcess(config->file_path);
        totalProc ++; //This counts the last process, which doesnot ends in semicolon
        

        //reading the meta-data file and storing data in heap using the procs struct
        procs *new = malloc(sizeof(procs) * totalProc);
        read_mdf(config->file_path, new); //all proceses loaded into new queue


        //logs for every event, an event is the a single line of output
        //logs store all data temporarily in heap, will be written in disk or monitor later
        char event[255];
        char* logs = malloc(sizeof(event)*(totalProc*3));
        

        double init_time = 0.000001; //starting time of the program
        double timeRequired;

        unsigned int address = 0x00000000; //starting address of the memory location in HEX
        //printf("0x%08x\n", address);

        //PCB pcb[totalProc];// process control block
        int counter = 1; //counting process going into execution
        int work = 0; //first load of work

        //resource counter
        int hdctr = 0; 
        int printctr = 0; 

        while(totalProc >= counter) //doesn't include last two processes (A and S)
        {
            //Where there's an I/O there's a thread
            if( strcmp(new[counter].proctag, "I") == 0 || strcmp(new[counter].proctag, "O") == 0 )
            {
                sem_init(&mutex, 1, 1);

                char *temp; //to see if write or read to hard disk
                if(strcmp(new[counter].proctag, "I") == 0)
                {
                    temp = "input";
                }
                else
                {
                    temp = "output";
                }

                if( strcmp(new[counter].procjob, "printer") == 0 )
                {
                    sem_wait(&mutex);
                    sprintf(event, "%f - Process %d: start printer output on PRNTR %d\n", init_time, work, printctr % config->prinquant);
                    strcat(logs, event);
                    printctr++;
                    timeRequired = runProcess(new[counter], config->printime, threadTrue);
                    init_time += timeRequired; 
                    sprintf(event, "%f - Process %d: end printer output\n", init_time, work);
                    strcat(logs, event);
                    sem_post(&mutex);
                }

                else if( strcmp(new[counter].procjob, "keyboard") == 0 )
                {
                    sem_wait(&mutex);
                    sprintf(event, "%f - Process %d: start keyboard input\n", init_time, work);
                    strcat(logs, event);
                    timeRequired = runProcess(new[counter], config->keytime, threadTrue);
                    init_time += timeRequired;
                    sprintf(event, "%f - Process %d: end keyboard input\n", init_time, work);
                    strcat(logs, event);
                    sem_post(&mutex);
                }

                else if( strcmp(new[counter].procjob, "monitor") == 0 )
                {
                    sprintf(event, "%f - Process %d: start monitor output\n", init_time, work);
                    strcat(logs, event);
                    timeRequired = runProcess(new[counter], config->montime, threadTrue);
                    init_time += timeRequired;
                    sprintf(event, "%f - Process %d: end monitor output\n", init_time, work);
                    strcat(logs, event);
                }

                else if( strcmp(new[counter].procjob, "harddrive") == 0 )
                {
                    sem_wait(&mutex);
                    sprintf(event, "%f - Process %d: start hard drive %s on HDD %d\n", init_time, work, temp, hdctr % config->hdquant);
                    strcat(logs, event);
                    hdctr++;
                    timeRequired = runProcess(new[counter], config->hardtime, threadTrue);
                    init_time += timeRequired;
                    sprintf(event, "%f - Process %d: end hard drive %s\n", init_time, work, temp);
                    strcat(logs, event);
                    sem_post(&mutex);
                }

                sem_destroy(&mutex);
            }

            else if(strcmp(new[counter].proctag, "P") == 0)
            {
                sprintf(event, "%f - Process %d: start processing action\n", init_time, work);
                strcat(logs, event);
                timeRequired = runProcess(new[counter], config->proctime, threadFalse); 
                init_time += timeRequired; 
                sprintf(event, "%f - Process %d: end processing action\n", init_time, work);
                strcat(logs, event);  
            }

            else if(strcmp(new[counter].proctag, "M") == 0)
            {
                if(strcmp(new[counter].procjob, "cache") == 0)
                {
                    sprintf(event, "%f - Process %d: start memory caching\n", init_time, work);
                    strcat(logs, event);
                    timeRequired = runProcess(new[counter], config->memtime, threadFalse);
                    init_time += timeRequired;
                    sprintf(event, "%f - Process %d: end memory caching\n", init_time, work);
                    strcat(logs, event);  
                }
                
                else if(strcmp(new[counter].procjob, "allocate") == 0)
                {
                    sprintf(event, "%f - Process %d: allocating memory\n", init_time, work);
                    strcat(logs, event);
                    timeRequired = runProcess(new[counter], config->memtime, threadFalse);
                    init_time += timeRequired;
                    address = allocateMem(new[counter], address, config->sysmem);
                    //printf("0x%08x\n", address);
                    sprintf(event, "%f - Process %d: memory allocated at 0x%08x\n", init_time, work, address);
                    strcat(logs, event);
                }
            }

            else if(strcmp(new[counter].proctag, "S") == 0)
            {
                if(strcmp(new[counter].procjob, "start") == 0)
                {
                    timeRequired = runProcess(new[counter], 0, threadFalse); //dafault iteration is 0
                    init_time += timeRequired;
                    sprintf(event, "%f - Simulator program starting\n", init_time);
                    strcat(logs, event);
                }
                else if(strcmp(new[counter].procjob, "end") == 0)
                {
                    timeRequired = runProcess(new[counter], 0, threadFalse); //dafault iteration is 0
                    init_time += timeRequired;
                    sprintf(event, "%f - Simulator program ending\n", init_time);
                    strcat(logs, event);
                }
            }

            else if(strcmp(new[counter].proctag, "A") == 0)
            {
                if(strcmp(new[counter].procjob, "start") == 0)
                {
                    work++; //starting line of work
                    sprintf(event, "%f - OS: preparing process %d\n", init_time, work);
                    strcat(logs, event);
                    timeRequired = runProcess(new[counter], 0, threadFalse);  
                    init_time += timeRequired; 
                    sprintf(event, "%f - OS: starting process %d\n", init_time, work);
                    strcat(logs, event);     
                }

                else if(strcmp(new[counter].procjob, "end") == 0)
                {
                    timeRequired = runProcess(new[counter], 0, threadFalse);  
                    init_time += timeRequired; 
                    sprintf(event, "%f - OS: removing process %d\n", init_time, work);
                    strcat(logs, event);     
                }
                
            }

            else
            {
                perror("Process not recognized: ");
            }

            counter++;
        }
        //printf("%s\n", logs);
        //printf("%s ===== %s\n", config->log, config->log_path);
        output(logs, config->log, config->log_path);
        free(logs);
        free(new);
        free(config);
    }
	return 0;
}







