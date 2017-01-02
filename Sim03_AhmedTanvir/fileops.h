/*
Operating System Simulator
created on: November 05, 2016
Author: Kazi Tanvir Ahmed Siddiqui
version: 3.0
*/

#ifndef FILEOPS_H_
#define FILEOPS_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void RemoveJunk(char* source, char seed)
{
  char* i = source;
  char* j = source;
  //printf("TEST\n");
  while(*j != 0)
  {
    *i = *j++;
    if(*i != seed)// || *i != '\n' || *i != "^M")
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

/*int pointeeSize(char *str)
{
  int i = 0;
  while(str[i] != NULL)
  {
    i++;
  }
  return i;
}*/

char* retString(char *str)
{
  int i = 0;
  char* temp;
  if(str[i]>65 && str[i]<90)
  {
    strcat(temp, &str[i]);
  }
  printf("%s\n", temp);
  return temp;
}

char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy (dest, input + offset, len);
  return dest;
}


#endif  /*FILEOPS_H_*/