#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"
#include "RunningProcesses.h"


/*Method to run a first come first serve scheduling routine*/
int runFCFS(Processes *proc, int time_interval)
{

  if(proc == NULL)
    {
      DEBUGPRINTF("ERROR: No process list passed to FCFS\n");
      exit(1);
    }
  RunningProcesses * rp = createRunningProcesses();
  addProc(rp,0);
  int runTime = time_interval;
  Node * temp = NULL;
  //decalre phase
  int block = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int current_time = 0;
  int finish = 0;
  int runProcReturn = 0;

  //initilize variables for printing
  int startArray[10000];
  int endArray[10000];
  int runArray[10000];
  int blocksArray[10000];
  double upArray[10000];
  int totalIdle = 0;
  int j = 0;
  for(j = 0; j <10000; j++)
    {
      startArray[j] = 0;
      endArray[j]= 0;
      runArray[j]=0;
      blocksArray[j]=0;
      upArray[j]=0;
    }

  while(rp->size != 0)//while there are still jobs to be run
    {
      //reset variables
      block = 0;
      arrival = 0;
      proc_arrival = 0;
      finish = 0;
      runProcReturn = 0;
      //
      temp = rp->head;
      runTime = time_interval;
      //find the shortest blocktime to run up too
      while (temp != NULL  && temp->isBlocked != 0)//0 is not blocked 1 is blocked
	{
	  if (temp->remainingBlockTime < runTime)
	    {
	      runTime = temp->remainingBlockTime;
	    }
	  temp = temp->next;
	}
      //NOTE
      //If TEMP==NULL then we can do run_noProc_check else if temp is a proc then do run_proc
      if (temp != NULL)//a process was found that could be run
	{
	  runProcReturn = run_proc(proc, temp->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);//run the process
	}
      else//there are no processes that can be run
	{
	  runProcReturn = proc_norun_check_arrival(proc, time_interval, current_time, &arrival, &proc_arrival);
	  totalIdle += runProcReturn;
	}
      if (runProcReturn != -1)
	{
	  current_time += runProcReturn;//increment current time
	}
      //arrival found
      if (arrival == 1)
	{
	  addProc(rp,proc_arrival);
	  startArray[proc_arrival] = current_time;
	}
      //block found
      if(block == 1)
	{
	  temp->isBlocked = 1;
	  blocksArray[temp->procID]++;
	}
      //finish process
      if (finish == 1)
	{
	  endArray[temp->procID] = current_time;
	  upArray[temp->procID] = runArray[temp->procID] / endArray[temp->procID];
	  runArray[temp->procID] = endArray[temp->procID] - startArray[temp->procID];
	  removeProc(rp,temp->procID);
	}
      
      if (rp->size != 0) // set temp back to the head
	{
	  temp = rp->head;
	}
      //increment all times
      while (temp != NULL  && temp->isBlocked != 0 && runProcReturn != -1)//0 is not blocked 1 is blocked
	{
	  if (temp->isBlocked == 1)
	    {
	      temp->remainingBlockTime -= runProcReturn;
	    }
	  if(temp->remainingBlockTime <= 0)//unblock processthat have done there time in the slammer
	    {
	      temp->isBlocked = 0;
	      temp->remainingBlockTime = 200;
	    }
	  temp = temp->next;
	}
    
      
    }

  // Fill in with your stats!
	int i = 0;
	printf("**********  SUMMARY ***********\n");
	printf("\tStart\tend\trun\tblocks\tutil percent\n");
	int sumRun = 0;
	for(i=0;endArray[i] != 0;i++)
	{
	  upArray[i] = (double)runArray[i] / (double) endArray[i];
		printf("PID%d:\t%d\t%d\t%d\t%d\t%1.3f\n",i,startArray[i],endArray[i],runArray[i],blocksArray[i],upArray[i]);
		sumRun += runArray[i];
	}
	printf("Mean Time to Finish:\t%d\n",(sumRun / i));
	printf("Total idle time:\t%d\n",totalIdle);
  return 0;
}
