#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int timePassed(struct timeval reference);

int main()
{
    int timeElapsed;
    struct timeval refTime;

    gettimeofday(&refTime, NULL);
    //^gets time of now since 1970 0:00:00 UTC and store at a place where refTime is pointing
    //printf("Ref time: %ld.%06d\n", refTime.tv_sec, refTime.tv_usec);

    printf("Test Print\n");
    timeElapsed = timePassed(refTime);
    printf("Time elapsed: %d\n", timeElapsed);
}

int timePassed( struct timeval refTime )
{
   struct timeval currentTime;
   int microsec, seconds;

   gettimeofday( &currentTime, NULL );
   seconds = currentTime.tv_sec - refTime.tv_sec;
   microsec = currentTime.tv_usec - refTime.tv_usec;

   if( microsec < 0 )
   {
       microsec += 1000000;
       seconds -= 1;
   }

   if( seconds > 0 )
   {
       microsec = microsec + ( seconds * 1000000 );
   }

   //printf( "Total time elapsed: %ld.%06d\n", seconds, microsec );
   //printf("%i\n", microsec);

   return microsec;
}
