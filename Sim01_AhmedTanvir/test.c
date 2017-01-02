#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


struct InfoConf {
	unsigned int proctime, montime, hardtime, printime, keytime, memtime, sysmem;
	char log[20], file_path[20], log_path[30], ver[10];
};

struct InfoMeta {
	/*Ss = System start, Se = System end, 
	As = Application start, Ae Application end, 
	P = Process, 
	Id = Input (hard drive) , Ik = Input (keyboard), 
	Od = Output (hard drive), Om = Output (monitor), Op = Output (printer)
	Ma = Memory allocate, Mc = Memory cache */

	int Ss, Se, As, Ae, P, Id, Ik, Od, Om, Op, Ma, Mc;
};

void RemoveSpaces(char* source);
char *small (char str[]);
//void read_file(char const *fileName)

int main ()
{	
	struct InfoConf configfile;
	struct InfoMeta metafile;

	char line[255];
    char *str1 = "processor cycle time (msec)";
    char *str2 = "monitor display time (msec)";
    char *str3 = "hard drive cycle time (msec)";
    char *str4 = "printer cycle time (msec)";
    char *str5 = "keyboard cycle time (msec)";
    char *str6 = "memory cycle time (msec)";
    char *str7K = "system memory (kbytes)";
    char *str7M = "system memory (mbytes)";
    char *str7G = "system memory (gbytes)";
    char *str8 = "log";
    char *str9 = "file path";
    char *str10 = "log file path";
    char *str11 = "version/phase";

    FILE *fp;
    fp = fopen("config_2.conf","r");

    if(fp == NULL) 
    {
 		perror("##Error in opening file: ");
        return(-1);
   	}

   	while(fgets(line, sizeof(line), fp) != NULL){
    	char* val1 = strtok(line, ":");
    	val1 = small(val1);
        char* val2 = strtok(NULL, ":");
        if(strcmp(str1, val1) == 0){
        	configfile.proctime = atoi(val2);
        }
        if(strcmp(str2, val1) == 0){
        	configfile.montime = atoi(val2);
        }
        if(strcmp(str3, val1) == 0){
        	configfile.hardtime = atoi(val2);
        }
        if(strcmp(str4, val1) == 0){
        	configfile.printime = atoi(val2);
        }
        if(strcmp(str5, val1) == 0){
        	configfile.keytime = atoi(val2);
        }
        if(strcmp(str6, val1) == 0){
        	configfile.memtime = atoi(val2);
        }
        if(strcmp(str7K, val1) == 0){
        	configfile.sysmem = atoi(val2); //For kilobyte of memory
        }
        if(strcmp(str7M, val1) == 0){
        	configfile.sysmem = 1000*(atoi(val2)); //For Megabytes of memory
        }
        if(strcmp(str7G, val1) == 0){
        	configfile.sysmem = 1000000*(atoi(val2)); //For Gigabytes of memory: No more than 2100 GB
        }
        if(strcmp(str8, val1) == 0){
        	strcpy(configfile.log, val2);
        }
        if(strcmp(str9, val1) == 0){
        	RemoveSpaces(val2);
        	strcpy(configfile.file_path, val2);
        }
        if(strcmp(str10, val1) == 0){
        	RemoveSpaces(val2);
        	strcpy(configfile.log_path, val2);
        }
        if(strcmp(str11, val1) == 0){
        	RemoveSpaces(val2);
        	strcpy(configfile.ver, val2);
        }
    }

   	fclose(fp);
   	
   	printf("*** %d", configfile.memtime);
   	printf("*** %d", configfile.montime);
   	printf("*** %d", configfile.printime);
   	printf("*** %d", configfile.hardtime);
   	printf("*** %d", configfile.keytime);
   	printf("*** %d", configfile.sysmem);
   	printf(">>>%s\n", configfile.log);
   	printf(">>>%s\n", configfile.file_path);
   	printf(">>>%s\n", configfile.log_path);
   	printf(">>>%s\n", configfile.ver);

   	//char *temp; 
   	//strncpy(temp, configfile.file_path, 12);
   	//printf("%s\n", temp);
   	FILE *fp1;
   	fp1 = fopen(configfile.file_path, "r");
   	//printf("%d\n", fp1);
   	char line1[255];
   	char *ptr1 = "s(start)";
   	char *ptr2 = "s(end)";
   	char *ptr3 = "a(start)";
   	char *ptr4 = "a(end)";
   	char *ptr5 = "p(run)";
   	char *ptr6 = "i(hard drive)";
   	char *ptr7 = "i(keyboard)";
   	char *ptr8 = "o(hard drive)";
   	char *ptr9 = "o(monitor)";
   	char *ptr10 = "o(printer)";
   	char *ptr11 = "m(allocate)";
   	char *ptr12 = "m(cache)";


   	if(fp1 == NULL){
   		perror("Error in opening Meta-data file: ");
   	}
   	while(fgets(line1, sizeof(line1), fp1) != NULL){
   		char *var1 = strtok(line1, ")");
   		//var1 = small(var1);
   		//const char *var2 = strtok(NULL, )

   	}
   	return(0);
}

void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

char *small(char str[]){
	for(int i = 0; str[i]; i++){
  		str[i] = tolower(str[i]);
	}
	return str;
}









