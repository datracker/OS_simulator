#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;


class configFile{
public:
	int proc, monitor, hdd, printer, keyboard, mem, sys_mem;
	string file_path, log, log_path, ver;
};

class metaFile{
public:
	/*Ss = System start, Se = System end, 
	As = Application start, Ae Application end, 
	P = Process, 
	Id = Input (hard drive) , Ik = Input (keyboard), 
	Od = Output (hard drive), Om = Output (monitor), Op = Output (printer)
	Ma = Memory allocate, Mc = Memory cache */
public:
	int Ss, Se, As, Ae, P, Id, Ik, Od, Om, Op, Ma, Mc;
};


void run(string f);
void output();
string small(string s);
int string2int(string s);
void write_file();
void write_monitor();



#endif