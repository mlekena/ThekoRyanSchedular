#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// include header files from the dictionary and word_list
#include "processes.h"
#include "FCFS.h"
#include "RR.h"
#include "SRT.h"
#include "MLQ.h"

#define MAX_JOBS 100
#define DEBUGPRINTF if(0){}else printf

int algorithm;
char * filename;

void print_usage()
{
printf("Usage: ./schedule <algorithm> <input file>\n");
printf("\tAlgorithms are\t0: Shortest Job Remaining\n");
printf("\t\t\t1: First Come First Serve\n");
printf("\t\t\t2: Round Robin\n");
printf("\t\t\t3: Multi-level queues\n");
exit(0);
}

void print_finished()
{
  /*// Fill in with your stats!
	int i = 0;
	printf("**********  SUMMARY ***********\n");
	printf("\tStart\tend\trun\tblocks\tutil percent\n");
	for(i=0;i<num_jobs;i++)
	{
		printf("PID%d:\t%d\t%d\t%d\t%d\t%1.3f\n",XX,XX,XX,XX,XX,XX);
	}
	printf("Mean Time to Finish:\t%d\n",XX);
	printf("Total idle time:\t%d\n",XX);
  */	
}

int main (int argc, char * argv [])
{

  int j = 1;
  algorithm = -1;
  if(argc != 5)
    {
      print_usage();
    }

	
  // process command line arguments
  while (j < argc) {
    if (strcmp ("-alg", argv [j]) == 0) {
      j++;
      if (j >= argc)
	print_usage ();
      algorithm = atoi (argv [j]);
      j++;
    } else if (strcmp ("-file", argv [j]) == 0) {
      j++;
      if (j >= argc)
	print_usage ();
      filename = argv [j];
      j++;
    }
    else{
      j++;
    }
  }
  if((algorithm == -1) || (filename==NULL))
    {
      print_usage();
    }

  //we'll always assume a basic quanta is 100 time units
  int time_step = 100;
  //int curr_time = 0;
  // create the system emulation of processes from the file
  Processes *proc = proc_create(filename);
/*
  int zero = 0;
  int done = 0;
  int blocked = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int count = 0;
  ////////////////////////////////////////////////////////////////////////////////////////////////
    while(count < 10)
    {
      count++;
      DEBUGPRINTF("ran test while\n");
      int returnVal = 0;
      if(returnVal == -1 || blocked == 1)
	{
	  returnVal = proc_norun_check_arrival(proc, time_step, curr_time, &arrival, &proc_arrival);
	  curr_time += returnVal;
	}
      else
	{
	  int returnVal = run_proc(proc, 0, time_step, &blocked, &done, curr_time, &zero, &zero);
	  curr_time += returnVal;
	}
      
      proc_print(proc);
      DEBUGPRINTF("Current Time = %d\n", curr_time);
      ////////////////////////////////////////////////////////////////////////////////////////////////
    }
  */
  //setup first job
  // int returnVal = runFCFS(proc, &time_step);
  // You may assume that there is a job 0 which arrived at time 0 and is ready to go


  // Here is the bulk of your work.  You will need to run until you've finished running all the
  // incoming processes.  (You may assume that when you have finished all the jobs you know about, you are
  // done.  Arrival times will always be early enough to ensure future processes have arrived before
  // you have finished previous ones.)

  //0 Shortest remaining time
  //1 First Come First Serve
  //2 Round Robin
  //3 Multi-level Queue
  switch(algorithm)
    {
    case 0: 
      runSRT(proc,time_step);
      break;
    case 1:
      runFCFS(proc,time_step);
      break;
    case 2:
      runRR(proc,time_step);
      break;
    case 3:
      runMLQ(proc,time_step);
      break;
    default:
      printf("Please enter valid scheduling argument. \"%d\" is not a valid option\n", algorithm);
      break;
    }
  
  

  print_finished();

  proc_destroy(proc);
  free(proc);
  proc=NULL;
  return 0;
}
