   #define _GNU_SOURCE
   #include <pthread.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <sys/types.h>
   #include <sys/time.h> 
   #include <unistd.h>
   #include <sys/wait.h>

	struct timeval t1, t2; /*structure for getting the elapsed time*/
	
	double elapsedTime; /* variable to store the elapsed time for different operations */
	double total_time_per_operation[10000]; /* variable to store the total elapsed time for each operation */
	double currentTime = 0; /* variable to store the currently elapsed total time */
	double processorTime, monitorTime, hddTime, printerTime, keyboardTime; /* variables to store different cycle times */
	
	char ts[100]; /* A temporary string to store the meta-data file name */
	char log_file[20]; /* A temporary string to store the log file name */
	char log_text[500000] = "";
	char option[10];
	char temp_string[BUFSIZ];
	
	struct v /*structure for passing data to threads*/
	{
		int indx; /* thread index */ 
		double cycles; /*number of cycles */
		double time_per_cycle; /*time per cycle */
	};
	
	
    /*The thread will begin control in this function. This is the prototype.*/
	void *runner(void *param);  
	
	char temp_str[100];

    /*The main() function. The thread creation and printing the output matrix is done here.*/
    int main(int argc, char *argv[])
    {	   	
		gettimeofday(&t1, NULL);
		
		FILE * fp;
		char * line = NULL;		
	    char filename[100];
		int line_count = 0;
		size_t len = 0;
        ssize_t read;
		int no_op=0; /* number of operations */
		pthread_t tid[10000]; /*thread identifiers*/
		pthread_attr_t attr; /*attributes for the threads*/
		pthread_attr_init(&attr); /*Setting the attributes for a thread*/
		
		char operations[10000][20];	/* Strings to store the operations */	
			
		gettimeofday(&t2, NULL);		
		
		/* Getting the elapsed time in microsecond accuracy */
		elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
		
		currentTime += elapsedTime;	
			

    		/*Start: Check If Input Filename is correctly provided at command line */
 
		if(argc==2)
			strcpy(filename, argv[1]);
		else if (argc>2) {
			printf("Just enter one file as input!\n");
		exit(1);
		}
		else {
		printf("Please enter the input file name.\n");
		exit(1);
		}  

    		/*End: Check If Input Filename is correctly provided at command line */

	 	/*Start: Open the configuration file for reading*/ 
 
		if((fp=fopen(filename, "r"))==NULL){
		printf("Cannot open file.\n");
		exit(1);
		}
	 
   	 	/*End: Open the input file for reading*/	

		/* Parsing the configuration file to get the necessary information*/
		while ((read = getline(&line, &len, fp)) != -1) {
			char temp_string[100];
			strncpy(temp_string, line, 4);
			if (!strcmp(temp_string,"File") || !strcmp(temp_string,"file"))	{
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				memcpy(ts, &line[index+1], strlen(line)-1-index);
				ts[strlen(ts)-1] = '\0';					
			}	
			
			if (!strcmp(temp_string,"Proc") || !strcmp(temp_string,"proc")) {
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				strncpy(temp_str, line+index+1, strlen(line)-index);
				processorTime = atof(temp_str)/1000;
				//printf("%.6f\n", processorTime);					
			}
			
			if (!strcmp(temp_string,"Moni") || !strcmp(temp_string,"moni")) {
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				strncpy(temp_str, line+index+1, strlen(line)-index);
				monitorTime = atof(temp_str)/1000;
				//printf("%.6f\n", monitorTime);					
			}
			
			if (!strcmp(temp_string,"Hard") || !strcmp(temp_string,"hard")) {
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				strncpy(temp_str, line+index+1, strlen(line)-index);
				hddTime = atof(temp_str)/1000;
				//printf("%.6f\n", hddTime);					
			}
			
			if (!strcmp(temp_string,"Prin") || !strcmp(temp_string,"prin")) {
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				strncpy(temp_str, line+index+1, strlen(line)-index);
				printerTime = atof(temp_str)/1000;
				//printf("%.6f\n", printerTime);					
			}
			
			if (!strcmp(temp_string,"Keyb") || !strcmp(temp_string,"keyb")) {
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				strncpy(temp_str, line+index+1, strlen(line)-index);
				keyboardTime = atof(temp_str)/1000;
				//printf("%.6f\n", keyboardTime);					
			}

			if (!strcmp(temp_string,"Log:") || !strcmp(temp_string,"log:"))	{
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				memcpy(option, &line[index+1], strlen(line)-1-index);
				option[strlen(option)-1] = '\0';					
			}
			
			if (!strcmp(temp_string,"Log ") || !strcmp(temp_string,"log "))	{
				int index=strlen(line)-1;
				while (line[index]!=' ') {
					index--;
				}
				memcpy(log_file, &line[index+1], strlen(line)-1-index);
				log_file[strlen(log_file)-1] = '\0';					
			}

			

			
		}	
		
		   
		fclose(fp); /*closing the configuration file */
		
		gettimeofday(&t1, NULL);
		
		/* Starting Simulator Program */
		char temp[50];
		sprintf(temp, "%.6lf - Simulator program starting\n", currentTime);
		if (strcmp(option,"File"))
			printf("%s", temp); 
		strcat(log_text, temp);
		
		gettimeofday(&t2, NULL);
		
		/* Getting the elapsed time in microsecond accuracy */
		elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
		
		currentTime += elapsedTime;
		
		gettimeofday(&t1, NULL);
		
		/* Preparing Process 1 */
		sprintf(temp, "%.6lf - OS: preparing process 1\n", currentTime); 
		if (strcmp(option,"File"))
			printf("%s", temp); 
		strcat(log_text, temp);
		
		gettimeofday(&t2, NULL);
		
		/* Getting the elapsed time in microsecond accuracy */
		elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
		
		currentTime += elapsedTime;
		
		gettimeofday(&t1, NULL);
		
		/* Starting Process 1 */
		sprintf(temp, "%.6lf - OS: starting process 1\n", currentTime); 
		if (strcmp(option,"File"))
			printf("%s", temp); 
		strcat(log_text, temp);
		
/* 		if (line)
		free(line); */
	
		/* opening meta-data file */
		if((fp=fopen(ts, "r"))==NULL){
		printf("Cannot open meta-data file, please check the file name in configuration file.\n");
		exit(1);
		}
		
		char ch;
		while((ch=fgetc(fp))!=EOF){
			if(ch==';') 
				no_op++;
		}
		
		no_op -= 3;
		
		//printf("%d\n", no_op);	
		
		fclose(fp); /* closing meta-data file */ 
		
		/* opening meta-data file again for parsing */
		if((fp=fopen(ts, "r"))==NULL){
			printf("Cannot open meta-data file, please check the file name in configuration file.\n");
			exit(1);
		}
		
		
/* 		for(int i = 0; i < no_op; i++) {
			operations[i] = (char*)malloc(15*sizeof(char));
		} */
		
		int index=0;
		int flag=0;
		while((ch=fgetc(fp))!=EOF){
			if(ch=='(') {
				flag=1;	
				continue;
			}
				if (flag==1 && ch=='r') {
					strcpy(operations[index],"processing");
					//printf("%s", operations[index]);					
					index++;
				}
				if (flag==1 && ch=='m') {
					strcpy(operations[index],"monitor");
					//printf("%s", operations[index]);					
					index++;
				}				
				if (flag==1 && ch=='h') {
					strcpy(operations[index],"hard drive");
					//printf("%s\n", operations[index]);
					index++;
				}				
				if (flag==1 && ch=='p') {
					strcpy(operations[index],"printer");
					//printf("%s\n", operations[index]);
					index++;
				}
				if (flag==1 && ch=='k') {
					strcpy(operations[index],"keyboard");
					//printf("%s\n", operations[index]);
					index++;
				}
				flag=0;			
		}
		
		fclose(fp); /* closing meta-data file */
		
		index=0;
		int counter=0,ind=0;
		int temp_c;
		double no_of_cycles[no_op]; /* no. of cycles in in operations */
		
		if((fp=fopen(ts, "r"))==NULL){
			printf("Cannot open meta-data file, please check the file name in configuration file.\n");
			exit(1);
		}	

		/* converting the meta-data file to a single string */
		while(index < BUFSIZ-1 && (temp_c = fgetc(fp)) != EOF)
			temp_string[index++] = temp_c;
		temp_string[index] = '\0'; 
		//printf("%s\n", temp_string);
		fclose(fp); /*closing meta-data file*/
		
		/* extracting no. of cycles per operation */
		for (counter=0;counter<strlen(temp_string);counter++) {
			if (temp_string[counter]==')' && temp_string[counter+1]!='0') {
				index=counter+1;
				while (temp_string[index]!=';') {
					index++;
				}
				strncpy(temp_str, temp_string+counter+1, index-counter);
				no_of_cycles[ind]=atof(temp_str);
				ind++;
			}
		}
		//printf("%.6f", no_of_cycles[8]);
		
		/* Creating the Threads */
		for (index=0;index<no_op;index++) {
			struct v *data = (struct v *)malloc(sizeof(struct v));
			data->indx = index;
			data->cycles = no_of_cycles[index];
			if (!strcmp(operations[index],"processing"))				
				data->time_per_cycle = processorTime;
			if (!strcmp(operations[index],"monitor"))				
				data->time_per_cycle = monitorTime;
			if (!strcmp(operations[index],"hard drive"))				
				data->time_per_cycle = hddTime;
			if (!strcmp(operations[index],"printer"))				
				data->time_per_cycle = printerTime;
			if (!strcmp(operations[index],"keyboard"))				
				data->time_per_cycle = keyboardTime;
			pthread_create(&tid[index],&attr,runner,(void* ) data); /*Create a thread for each operation*/
		}
		
		/*Joining the threads so that the parent thread waits for them to complete.*/
		for(index=0;index<no_op;index++)  {
			pthread_join(tid[index], NULL); /*wait for the thread to exit*/
		}
		
		/* displaying the output of operations*/
		for(index=0;index<no_op;index++)  {
			gettimeofday(&t2, NULL);			
			/* Getting the elapsed time in microsecond accuracy */
			elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
			elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
			
			currentTime += elapsedTime;
			
			if (!strcmp(operations[index],"processing")) {
				sprintf(temp,"%.6f - Process 1 : start %s action\n", currentTime, operations[index]);
				if (strcmp(option,"File"))
					printf("%s", temp); 
				strcat(log_text, temp);				
			}
			else {
				sprintf(temp,"%.6f - Process 1 : start %s input\n", currentTime, operations[index]);
					if (strcmp(option,"File"))
						printf("%s", temp); 
					strcat(log_text, temp);
			}
			
			
			currentTime += total_time_per_operation[index];
			
			if (!strcmp(operations[index],"processing")) {
				sprintf(temp,"%.6f - Process 1 : end %s action\n", currentTime, operations[index]);
				if (strcmp(option,"File"))
					printf("%s", temp); 
				strcat(log_text, temp);
			}
			else {
				sprintf(temp,"%.6f - Process 1 : end %s input\n", currentTime, operations[index]);
				if (strcmp(option,"File"))
					printf("%s", temp); 
				strcat(log_text, temp);
			}
			
			gettimeofday(&t1, NULL);	
		}
		
		gettimeofday(&t2, NULL);			
		/* Getting the elapsed time in microsecond accuracy */
		elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
		
		gettimeofday(&t1, NULL);
		currentTime += elapsedTime;
		
		/* action of removing the process */
		sprintf(temp,"%.6f - OS : removing process 1\n", currentTime);
		if (strcmp(option,"File"))
			printf("%s", temp); 
		strcat(log_text, temp);
					
		gettimeofday(&t2, NULL);			
		/* Getting the elapsed time in microsecond accuracy */
		elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
		currentTime += elapsedTime;
		
		/* simulator program ending */
		sprintf(temp,"%.6f - Simulator program ending\n", currentTime);		
		if (strcmp(option,"File"))
			printf("%s", temp); 
		strcat(log_text, temp);
		//printf("%s\n", log_file);		
		/* opening log_file for writing */ 

		if (strcmp(option,"Monitor")) {
			FILE *new_log = fopen(log_file, "w");
			fputs(log_text, new_log);
			fclose(new_log);
		}
/*		
		FILE *new_log = fopen(log_file, "w");
		fputs(log_text, new_log);
		fclose(new_log);
*/
		//printf("%s\n", log_file);
		//freopen(log_file, "w", stdout);
		//fclose(fp);
	
		exit(EXIT_SUCCESS);
	}
	
	/* Thread function for calculating the total elapsed time of each operation */
	void *runner(void* param) {
		int temp_index;		
		struct v *datam = param;
		double x,y;
		temp_index = datam->indx;
		x = datam->cycles;
		y = datam->time_per_cycle;
		total_time_per_operation[temp_index] = x*y;
		 
		pthread_exit(0);
	}
