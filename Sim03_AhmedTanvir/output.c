/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#include "output.h"

void output(char *stream, char *destination, char *fileName)
{
	if(strcmp(destination, "logtofile") == 0)
	{
		FILE *fp;
  		fp = fopen(fileName,"w");
  		fputs(stream, fp);
  		fclose(fp);
	}

	else if(strcmp(destination, "logtomonitor") == 0)
	{
		printf("%s\n", stream);
	}
	
	else if(strcmp(destination, "logtoboth") == 0)
	{
		printf("%s\n", stream);
		FILE *fp;
  		fp = fopen(fileName,"w");
  		fputs(stream, fp);
  		fclose(fp);
	}
}