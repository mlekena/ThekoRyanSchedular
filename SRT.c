#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"
#include "RunningProcesses.h"




int runSRT(Processes *proc, int time_interval)
{

  if(proc == NULL)
    {
      DEBUGPRINTF("ERROR: No process list passed to FCFS\n");
      exit(1);
    }
  RunningProcesses * rp = createRunningProcesses();
  addProc(rp,0);
  int minTime = 999999999;//set min time to large number to start
  Node * temp = NULL;
  Node * currProc = NULL;//process to be run

  int block = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int current_time = 0;
  int finish = 0;
  int runProcReturn = 0;
  int runTime = time_interval;

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

  while(rp->size != 0)//assumption that once this is true all processes will have finished
    {

      minTime = 999999999;//set min time to large number to start
      //reset
      block = 0;
      arrival = 0;
      proc_arrival = 0;
      finish = 0;
      runProcReturn = 0;
      currProc = rp->head;

      temp = rp->head;
      runTime = time_interval;
      //finds the process with the shortest remaining run time
      //printf("the id of the HEAD: %d\n", temp->procID);
      //printf("SIZE = %d\n", rp->size);
      if(rp->size < 1)
	{
	  return 0;
	}

      while(temp != NULL)
	{
	  // printf("temp is not null\n");
	 
	  if(temp->isBlocked == 1 && temp->remainingBlockTime < runTime)
	    {
	      runTime = temp->remainingBlockTime;
	    }
	  if(temp->isBlocked == 0 && proc_time_remaining(proc, temp->procID) < minTime)
	    {
	      currProc = temp;
	      minTime = proc_time_remaining(proc, temp->procID);
	    }
	  temp = temp->next;
	}
      // printProcesses(rp);
      // printf("out the loop\n");

      //NOTE
      //If CURRPROC==NULL then we can do run_noProc_check else if currProc is a proc then do run_proc
      if (currProc != NULL)
	{
	  //printf("ProcID = %d\n", currProc->procID);
	  runProcReturn = run_proc(proc, currProc->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);
	}
      else
	{
	  //printf("Im about to do a proc_norun_check_arrival\n");
	  runProcReturn = proc_norun_check_arrival(proc, time_interval, current_time, &arrival, &proc_arrival);
	  totalIdle += runProcReturn;
	  //printf("Proc No Run");
	}
      //increment current time
      //printf("run proc return = %d\n", runProcReturn);
      if (runProcReturn != -1)
	{
	  current_time += runProcReturn;
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
	  currProc->isBlocked = 1;
	  blocksArray[currProc->procID]++;
	}
      //finish process
      if (finish == 1)
	{
	  endArray[currProc->procID] = current_time;
	  upArray[currProc->procID] = runArray[currProc->procID] / endArray[currProc->procID];
	  runArray[currProc->procID] = endArray[currProc->procID] - startArray[currProc->procID];
	  removeProc(rp,currProc->procID);
	  
	}
      
      if (rp->size != 0)
	{
	  temp = rp->head;
	}


      //increment all times
      while (temp != NULL  && temp->isBlocked != 0 && runProcReturn != -1)//0 is not blocked 1 is blocked
	{
	  // printf("########################################\n");
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
    
      //printProcesses(rp);
      //proc_print(proc);      
      
    }
  //printProcesses(rp);
//proc_print(proc);

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
