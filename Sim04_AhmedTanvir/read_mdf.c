/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#include "read_mdf.h"
#include "fileops.h"


void read_mdf(char const *filename, procs *Proc)
{
  	char line1[255];

	  FILE *fp1;
  	fp1 = fopen(filename, "r");
   	unsigned int proc_ctr = 0;
   	//Proc[proc_ctr].pid = 1000; //initializing the first processID

  	while(fgets(line1, sizeof(line1), fp1) != NULL)
  	{
    	char *token;
    	token = strtok(line1, ";");

    	if(token == NULL)
    	{
    		perror("Error in Meta-data file content: ");
    	}
    	
    	while(token != NULL)
    	{
    		RemoveJunk(token, '\n');
    		RemoveJunk(token, ' ');
    		int flag1 = 0;
    		int flag2, flag3;
    		//printf("%c\n", token[flag1]);

    		if (token[flag1] == '\0')
    		{
    			//perror("Error in meta-data file token: ");
    			//printf("***%s\n", token);
    			proc_ctr--;
    		}

    		while(token[flag1] != '\0')
    		{
    			if(token[flag1] == '(')
    			{
    				memcpy(Proc[proc_ctr].proctag, token, flag1);
    				//printf("%s***%u\n", Proc[proc_ctr].proctag, proc_ctr);
    				flag2 = flag1;
    				
    				while(token[flag2] != ')')
    				{
    					flag2++;
    				}
    				if(token[flag2] == ')')
    				{
    					int offset = flag2 - flag1 -1;
    					memcpy(Proc[proc_ctr].procjob, token+flag1+1, offset);
    					//printf("%s###%u\n", Proc[proc_ctr].procjob, proc_ctr);
    					flag3 = flag2;

    					while(token[flag3] != '\0')
    					{
    						flag3++;
    					}
    					if(token[flag3] == '\0')
    					{
    						char ptr[5] = {'\0','\0','\0','\0','\0'};
    						int off = flag3 - flag2 -1;
    						memcpy(ptr, token+flag2+1, off);
							  sscanf(ptr, "%u", &Proc[proc_ctr].num_it);
    						//printf("%u>>>%u\n", Proc[proc_ctr].num_it, proc_ctr);
    					}	
    				}
    			}
    			flag1++;
    		}
    		proc_ctr++;
    		//Proc[proc_ctr].pid = 1000 + proc_ctr;
    		

      		token = strtok(NULL, ";");		
    	}
    }


  	fclose(fp1);

  	//printf("Process ctr: %u\n", proc_ctr
  	/*for(int i =1; i < proc_ctr - 1; i++)
  	{
  	  //printf("Process ID: %u\n", Proc[i].pid);
  	  printf("Process Ite: %u\n", Proc[i].num_it);
	  	printf("Process name: %s\n", Proc[i].proctag);
    	printf("Process job: %s\n", Proc[i].procjob);

  	}*/
  	
  //return Proc; 
  //return 0;

}


void load_app(apps *app, procs *proc, int total)
{
  //printf("%s\n", proc[76].proctag);
  int start = 0;
  int app_ctr = 0;
  int i = 0;
  while(start <= total)
  {
    if( (!strcmp(proc[start].proctag, "A")) || (!strcmp(proc[start].proctag, "P")) || (!strcmp(proc[start].proctag, "I")) || (!strcmp(proc[start].proctag, "O")) || (!strcmp(proc[start].proctag, "M")) )
    {
      if (strcmp(proc[start].procjob, "end"))
      {
        app[app_ctr].process[i] = proc[start]; 
        i++;
      }
      else
      {
        app[app_ctr].process[i] = proc[start]; 
        app_ctr++;
        i = 0;
      }
    }
    start++;
  }
  //printf("%s>>%s>>%d \n", app[1].process[21].proctag, app[1].process[21].procjob, app[1].process[21].num_it);
}















