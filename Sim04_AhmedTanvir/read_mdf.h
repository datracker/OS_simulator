/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#ifndef READ_MDF_H_
#define READ_MDF_H_

typedef struct {
	//pid = process id; num_it = number of times the process run; procname = process name
	int num_it; 
	char proctag[5], procjob[20];
}procs;

typedef struct {
	procs *process;
	unsigned int cycle_remain, is_finished;
}apps;

/*
typedef struct {
	procs *process;
	char *logger;
	unsigned int runtime, totaltime, app_num, res_num, total_res, cycle_left;
	double timePassed, prev_time;
}newProc;
*/

void read_mdf(char const *filename, procs *Proc);
void load_app(apps *app, procs *proc, int total);

#endif /* End of READ_MDF_H*/