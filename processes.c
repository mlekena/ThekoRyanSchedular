/* Processes.c
 *
 * Stores a list of processes.
 * Author: L. Porter
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"

#define INIT_SIZE 1024


// represents a single job
struct Proc{
	int start_time;		// start time
	int array[INIT_SIZE];	// array of time intervals to run between blocking events
	int num_events;		// how many elements are in the array
	int curr_event;		// which "event" (index into array) is currently running
	int progress;		// how much progress you've made in that event
				// progress<= array[curr_event]
	int done;		// if this process is done this is 1, else 0
	int announced;		// if you've announced this process to the parent, mark as 1
	int total_time;		// total time required to finish this job (just the sum of array)
	int total_progress;	// total time you've run this job
				// total_time - total_progress is needed to report time remaining efficiently
};

// struct for all the jobs
struct Processes{
   struct Proc *array;		// array of Processes
   int internal_size;		// how big the array is total
   int size;			// how many valid Procs in the array
};
 
Processes * proc_create(char *filename)
{
   // This function is provided for you.  You will need to read through it to better understand
   // how this is setup and how to write the missing functions.
   Processes * retval = malloc(sizeof(Processes));

   int i =0,j=0,total_time=0;
   DEBUGPRINTF("Creating Processes\n");
   retval->array = malloc(sizeof(struct Proc)*INIT_SIZE);
   retval->internal_size = INIT_SIZE;
   retval->size = 0;

   {
   // fill the array, walk 1 by 1

   FILE * istream;
   istream = fopen(filename, "r");
   
   char id[55];
   char time[55];
   
   i=-1;  // needed for this next step
   j = 0;
   while (fscanf(istream, "%s\t%s",id, time)!=EOF && i<INIT_SIZE-1)
   {
	int proc_id =atoi(id);
	if(proc_id == i)
	{
		if(j>=INIT_SIZE)
		{
			fprintf(stderr,"ERROR - too many time_steps %d\n",i);
			fflush(0);
			exit(1);

		}
		//found match, just add this word
		DEBUGPRINTF("SAME PROC ID: %d\n",proc_id);
		fflush(0);
		retval->array[i].array[j] = atoi(time);
		total_time+=atoi(time);
		retval->array[i].num_events++;
		retval->array[i].total_time = total_time;
		j++;
	}
	//We are at first line of new process
	else
	{
		i++;
		if(i>=INIT_SIZE)
		{
			fprintf(stderr,"ERROR - too many jobs %d\n",i);
			fflush(0);
			exit(1);
		}
		retval->array[i].start_time = atoi(time);
		retval->array[i].num_events = 0;
		retval->array[i].curr_event = 0;
		retval->array[i].progress = 0;
		retval->array[i].total_progress = 0;
		retval->array[i].done = 0;
		if(i==0)
		{
			retval->array[i].announced = 1;
		}
		else
		{
			retval->array[i].announced = 0;
		}
		j=0;
		total_time = 0;
	}
	DEBUGPRINTF("Added %s\t%d\n",id, atoi(time));
   }
   DEBUGPRINTF("Total time required to run all%d\n",total_time);
   retval->size=i;
   fclose(istream);
   return retval;
   }
}
void proc_print(Processes *proc)
{
   // this function is provided to you and may help you with debugging
   // and/or understanding your programs.
   int i,j, end;
   for(i = 0;i<=proc->size;i++)
   {
	   if(proc->array[i].announced)
	   {
		   if(proc->array[i].done)
		   {
			   printf("* ");
		   }
		   printf("PID: %d\t",i);
		   end = proc->array[i].num_events;
		   for(j=0;j<end;j++)
		   {
			   printf("%d\t",proc->array[i].array[j]);
		   }
		   printf("\n");
		   printf("Curr Event = %d. ",end = proc->array[i].curr_event);
		   printf("Curr Progress = %d. ",end = proc->array[i].progress);
		   printf("Total Progress = %d. ",end = proc->array[i].total_progress);
		   printf("Total Runtime = %d.\n ",end = proc->array[i].total_time);
	   }
   }
}

int proc_norun_check_arrival(Processes *proc, int time_interval, int current_time, int *arrival, int *proc_arrival)
{
DEBUGPRINTF("GOT IN TO ProcNoRun\n");
  // YOU NEED TO COMPLETE THIS
  // See run_proc for directions.  In this case, you aren't running any processes, but you need to report if
  // a process arrives during this time interval
  int i = 0;
  int nextProc_id = -1;
  int timeUntilEvent = time_interval; 
  for(i = 0; i < proc->size + 1; i++)
    {
      if(proc->array[i].announced == 0)
	{
	  nextProc_id = i; 
	  break;
	}
    }
  if(nextProc_id != -1 && proc->array[nextProc_id].start_time <= current_time + time_interval)
    {
      DEBUGPRINTF("A process arrived during proc_norun_check_arrival with ID--->> %d\n", nextProc_id);
      
      timeUntilEvent = proc->array[nextProc_id].start_time - current_time;
      *arrival = 1;
      *proc_arrival = nextProc_id;
      proc->array[nextProc_id].announced = 1;
      return timeUntilEvent;
    }
  DEBUGPRINTF("Out Of ProcNoRun with NO ARRIVALS\n");
  return timeUntilEvent;
}

 int findShortest(arrivalTime, blockTime, timeInterval)
 {
   int timeArray[] = {arrivalTime, blockTime, timeInterval};
   int i =0;
   int indexOfShortest = 0;
   for (i = 0; i < 3; i++)
     {
       if(timeArray[indexOfShortest] >= timeArray[i])
	 {
	   indexOfShortest = i;
	 }
     }

  
   //0 means arrivalTime happend first
   //1 means blockTime happend first
   //2 means we ran to the end of the time interval 
   if(blockTime < 99999)
     {
       DEBUGPRINTF("arrivalTimee = %d, blockTime = %d\n", arrivalTime, blockTime);
     }
   return indexOfShortest;
 }
 int run_proc(Processes * proc, int proc_id, int time_interval, int * block, int * finish, int current_time, int *arrival, int *proc_arrival)
 {

   int arrivalTime = 999999, blockTime = 99999;
   // YOU NEED TO COMPLETE THIS
   // return -1 if proc is invalid or proc_id is invalid.
   if(proc == NULL || proc->size == 0 || proc_id < 0 || proc_id > proc->size)
     {
       DEBUGPRINTF("ERROR: Proc or Process id was invalid\n");
       return -1; 
     }

   // return -1 if this job is already done (you shouldn't be trying to run finished jobs)
   if(proc->array[proc_id].done == 1)
     {
       DEBUGPRINTF("ERROR: did not run job. It had already finished.\n");
       *finish = 1;
       return -1;
     }
   ///Checks happen here
   // check to see if anyone arrives during this time interval
   int i = 0;
   int nextProc_id = -1;
   for(i = proc_id+1; i <= proc->size; i++)
     {

       
       if(proc->array[i].announced == 0)
	 {

	   //printf("About to assume i is next proc: %d\n", i);
	   nextProc_id = i; 
	   break;
	 }
     }
   if(nextProc_id != -1 && proc->array[nextProc_id].start_time <= current_time + time_interval)
     {
       arrivalTime = proc->array[nextProc_id].start_time - current_time;
     }
   // See if the process blocks during this time interval
   DEBUGPRINTF("Returning remaning time for current event %d\n",proc->array[proc_id].array[proc->array[proc_id].curr_event] - proc->array[proc_id].progress);
   if(proc->array[proc_id].array[proc->array[proc_id].curr_event] - proc->array[proc_id].progress <= time_interval)
     {
       //blockTime = (proc->array[proc_id].array[proc->array[proc_id].curr_event] + current_time) -  proc->array[proc_id].start_time;
       blockTime = proc->array[proc_id].array[proc->array[proc_id].curr_event] - proc->array[proc_id].progress;
     }

  
   //0 means arrivalTime happend first
   //1 means blockTime happend first
   //2 means we ran to the end of the time interval
   int index_of_shortest = findShortest(arrivalTime, blockTime, time_interval);

   //////////////////////////////////////////////////////////////////////////////////////////////
   // check to see if anyone arrives during this time interval
   if(index_of_shortest == 0)
     {
       DEBUGPRINTF("A process arrived during run_proc\n");
       proc->array[proc_id].progress += proc->array[nextProc_id].start_time - current_time;
       proc->array[proc_id].total_progress += proc->array[nextProc_id].start_time - current_time;
       proc->array[nextProc_id].announced = 1; 
       *arrival = 1;
       *proc_arrival = nextProc_id;
       return proc->array[nextProc_id].start_time - current_time;
     }

   // See if the process blocks or finishes during this time interval
   else if (index_of_shortest == 1)
     {
       DEBUGPRINTF("**Process reached end of current event");
       //proc->array[proc_id].progress = proc->array[proc_id].array[proc->array[proc_id].curr_event];
       proc->array[proc_id].total_progress += proc->array[proc_id].array[proc->array[proc_id].curr_event] - proc->array[proc_id].progress;
      
       proc->array[proc_id].progress = 0;
       proc->array[proc_id].curr_event += 1;
       //if finishes
       if(proc->array[proc_id].total_progress >= proc->array[proc_id].total_time)
	 {
	   DEBUGPRINTF("process finished running during run_proc.\n");
	   *finish = 1;
	   proc->array[proc_id].done = 1; 
	  
	 }
       //if blocks
       else
	 {
	   DEBUGPRINTF("***A blocking event occured during run_proc.\n");
	   *block = 1;
	 }
       DEBUGPRINTF("about to return: %d\n", blockTime);
       return blockTime;
     }

   //if we ran to end of time interval 
   else if (index_of_shortest == 2)
     {
       DEBUGPRINTF("Ran to end of time interval\n");
       current_time += time_interval;
       proc->array[proc_id].total_progress += time_interval;
       proc->array[proc_id].progress += time_interval;
       /*if(proc->array[proc_id].total_progress >= proc->array[proc_id].total_time)
	 {
	 DEBUGPRINTF("process finished running during run_proc.\n");
	 *finish = 1;
	 proc->array[proc_id].done = 1; 
	  
	 }*/
       return time_interval;
     }
   // run to the shortest of (someone arriving, someone blocking, time_interval).
   //
   // if you ran the whole time interval without an event. You are done.
   // if you blocked, be sure to set the variables properly to notify the parent that a blocking event
   // occurred
   // 
   // if you encountered a new process, be sure to mark that process as having been announced and be sure
   // to set the variables properly to notify the parent that a new process arrived.
   // return how long you ran.
   return -1;
 }

 int proc_time_remaining(Processes *proc, int proc_id)
 {
   // this function is provided to you for your shortest_remaining_time schedule
   if(proc == 0)
     {
       return -1;
     }
   if(proc_id <0 || proc_id > proc->size)
     {
       return -1;
     }
   if(proc->array[proc_id].done == 1)
     {
       return 0;
     }
   return proc->array[proc_id].total_time - proc->array[proc_id].total_progress;
 }

 void proc_destroy(Processes *proc)
 {
   // clean up the data structure here.
   free(proc->array);
 }

