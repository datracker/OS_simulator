/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#ifndef OUTPUT_C_
#define OUTPUT_C_

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

#endif /* End of OUTPUT_C*/