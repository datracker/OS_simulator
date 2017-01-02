/*
file_read.h = Header file for file_read.c
created on: 
Author: Kazi Tanvir Ahmed Siddiqui
version: 1.0
*/
#ifndef FILE_READ_H_
#define FILE_READ_H_


typedef struct {
	unsigned int proctime, montime, hardtime, printime, keytime, memtime, sysmem, memblock, prinquant, hdquant;
	char log[20], file_path[20], log_path[30], ver[10];
}InfoConf;

void read_config(const char fileName[], InfoConf *configfile);

#endif  /*FILE_READ_H_*/