/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#ifndef READ_MDF_H_
#define READ_MDF_H_

typedef struct {
	//pid = process id; num_it = number of times the process run; procname = process name
	unsigned int pid, num_it; 
	char proctag[5], procjob[20];
}procs;

void read_mdf(char const *filename, procs *Proc);

#endif /* End of READ_MDF_H*/