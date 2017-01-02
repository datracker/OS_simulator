/*
Operating System Simulator
created on: December 16, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 4.0
*/

#include "file_read.h"
#include "fileops.h"

void read_config(const char fileName[], InfoConf *configfile)


//int main (char const *fileName, struct InfoConf *config)
{	
	//struct InfoConf *configfile = malloc(sizeof(struct InfoConf)); //initializing a struct and will store all the data here

  char str1[] = "processor cycle time (msec)";
  char str2[] = "monitor display time (msec)";
  char str3[] = "hard drive cycle time (msec)";
  char str4[] = "printer cycle time (msec)";
  char str5[] = "keyboard cycle time (msec)";
  char str6[] = "memory cycle time (msec)";
  char str7K[] = "system memory (kbytes)";
  char str7M[] = "system memory (mbytes)";
  char str7G[] = "system memory (gbytes)";
  char str8K[] = "memory block size (kbytes)";
  char str8M[] = "memory block size (mbytes)";
  char str8G[] = "memory block size (gbytes)";
  char str9[] = "printer quantity";
  char str10[] = "hard drive quantity";
  char str11[] = "log";
  char str12[] = "file path";
  char str13[] = "log file path";
  char str14[] = "version/phase";
  char str15[] = "processor quantum number";
  char str16[] = "cpu scheduling code";

  FILE *fp;
  fp = fopen(fileName,"r");

  if(fp == NULL) {
 		perror("Error in opening config file: ");
    //return(-1);
  }

  char line[255];
  while(fgets(line, sizeof(line), fp) != NULL){
    char* val1 = strtok(line, ":");
    val1 = small(val1);
    char* val2 = strtok(NULL, ":");

    if(strcmp(str1, val1) == 0){
      if(val2 != NULL){
        configfile->proctime = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str2, val1) == 0){
      if(val2 != NULL){
        configfile->montime = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str3, val1) == 0){
      if(val2 != NULL){
        configfile->hardtime = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str4, val1) == 0){
      if(val2 != NULL){
        configfile->printime = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str5, val1) == 0){
      if(val2 != NULL){
        configfile->keytime = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str6, val1) == 0){
      if(val2 != NULL){
        configfile->memtime = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str7K, val1) == 0){
      if(val2 != NULL){
        configfile->sysmem = atoi(val2);  //For kilobyte of memory   
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str7M, val1) == 0){
      if(val2 != NULL){
        configfile->sysmem = 1000*(atoi(val2));  //For Megabyte of memory   
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str7G, val1) == 0){
      if(val2 != NULL){
        configfile->sysmem = 1000000*(atoi(val2)); //For Gigabytes of memory: No more than 2100 GB
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str8K, val1) == 0){
      if(val2 != NULL){
        configfile->memblock = atoi(val2);  //For kilobyte of memory block  
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str8M, val1) == 0){
      if(val2 != NULL){
        configfile->memblock = 1000*(atoi(val2));  //For Megabytes of memory block
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str8G, val1) == 0){
      if(val2 != NULL){
        configfile->memblock = 1000000*(atoi(val2));  //For Gigabytes of memory block
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str9, val1) == 0){
      if(val2 != NULL){
        configfile->prinquant = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str10, val1) == 0){
      if(val2 != NULL){
        configfile->hdquant = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str11, val1) == 0){
      if(val2 != NULL){
        RemoveJunk(val2, ' ');
        RemoveJunk(val2, '\n');
        val2 = small(val2);
        strcpy(configfile->log, val2);
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }
    }
    if(strcmp(str12, val1) == 0){
      if(val2 != NULL){
        RemoveJunk(val2, ' ');
        RemoveJunk(val2, '\n');
        strcpy(configfile->file_path, val2);
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }
    }
    if(strcmp(str13, val1) == 0){
      if(val2 != NULL){
        RemoveJunk(val2, ' ');
        RemoveJunk(val2, '\n');
        strcpy(configfile->log_path, val2);
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }
    }
    if(strcmp(str14, val1) == 0){
      if(val2 != NULL){
        RemoveJunk(val2, ' ');
        RemoveJunk(val2, '\n');
        strcpy(configfile->ver, val2);
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }
    }
    if(strcmp(str15, val1) == 0){
      if(val2 != NULL){
        configfile->qnum = atoi(val2);          
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }   
    }
    if(strcmp(str16, val1) == 0){
      if(val2 != NULL){
        RemoveJunk(val2, ' ');
        RemoveJunk(val2, '\n');
        strcpy(configfile->scode, val2);
      }
      else{
        printf("Error in %s, value not found\n", val1);
      }
    }
  }

  fclose(fp);

  /*
  printf("***%d\n", configfile->proctime);
  printf("***%d\n", configfile->montime);
  printf("***%d\n", configfile->printime);
  printf("***%d\n", configfile->hardtime);
  printf("***%d\n", configfile->keytime);
  printf("***%d\n", configfile->sysmem);
  printf(">>>%s", configfile->log);
  printf(">>>%s", configfile->file_path);
  printf(">>>%s\n", configfile->log_path);
  printf(">>>%s\n", configfile->ver);
  printf(">>>%d\n", configfile->qnum);
  printf(">>>%s\n", configfile->scode);
  */

  //return configfile; //returning the address of the struct where all the data are stored
}










