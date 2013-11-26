#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"
#include "RunningProcesses.h"

int runRR(Processes *proc, int time_interval)
{
  if(proc == NULL)
    {
      DEBUGPRINTF("ERROR: No process list was passed to RR.Exiting gracefully\n");
      exit(1);
    }
  RunningProcesses * rp = createRunningProcesses();
  addProc(rp,0);
  
  //declaration phase
  Node * currProc = rp->head;
  Node * lastToRun = rp->head;
  Node * nextToLookAt = NULL;
  Node * temp = rp->head;
  int block = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int current_time = 0;
  int finish = 0;
  int runProcReturn = 0;
  int sumRunTime = 0;
  int runTime = time_interval;
  int count = 0;

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
  //scheduling phase
  while(/*count < 1000*/  rp->size != 0)
    {
      count++;
      //reset
      if (nextToLookAt != NULL)
	{
	  lastToRun = nextToLookAt;
	  nextToLookAt = NULL;
	}
      block = 0;
      arrival = 0;
      proc_arrival = 0;
      finish = 0;
      runProcReturn = 0;
      sumRunTime = 0;
      runTime = time_interval;
      if (lastToRun->procID > 100)
	{

	  printf("hi\n");
	}
      // printProcesses(rp);
      //proc_print(proc);
      //loop through the array 
	do{
	  //	  printf("<<<<<<<<<<<<<<<<<<<  current proc ID = %d block:%d blocktime %d lastToRun id:%d\n", currProc->procID,currProc->isBlocked,currProc->remainingBlockTime,lastToRun->procID);
	  if(currProc->next != NULL)
	    {
	      currProc = currProc->next;
	    }
	  else
	    {
	      currProc = rp->head;
	    }
	}while(currProc->isBlocked == 1 && currProc != lastToRun);
      //if a process that can be run is found run it till time_interval or its blocked or finishes.
      if(currProc->isBlocked == 0 )
	{
	  
	  lastToRun = currProc;
	  while (sumRunTime < time_interval)
	    {
	      // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ sumRunTimeof :%d must be less than time_interval of: %d\n", sumRunTime,time_interval);
	      //printf("Sum Time = %d", sumRunTime);
	      block = 0;
	      arrival = 0;
	      proc_arrival = 0;
	      finish = 0;

	      runProcReturn = run_proc(proc, currProc->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);
	     
	      
	      if (runProcReturn != -1)
		{
		  current_time += runProcReturn;
		  runTime -= runProcReturn;
		  sumRunTime += runProcReturn;//measures the time till currproc turn is up
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
		  break;
		}
	      //finish process
	      if (finish == 1)
		{
		  endArray[currProc->procID] = current_time;
		  upArray[currProc->procID] = runArray[currProc->procID] / endArray[currProc->procID];
		  runArray[currProc->procID] = endArray[currProc->procID] - startArray[currProc->procID];
		  nextToLookAt = currProc->next;
		  removeProc(rp,currProc->procID);
		  break;
		}
	      
	    }//end of turn while
	}//end of if
      //all announced processes are blocked so norun-check-arrival should be run
      else
	{
	  runProcReturn = proc_norun_check_arrival(proc, time_interval, current_time, &arrival, &proc_arrival);
	  totalIdle += runProcReturn;
	  current_time += runProcReturn;
	  sumRunTime = runProcReturn;
	  //arrival found
	  if (arrival == 1)
	    {
	      addProc(rp,proc_arrival);
	    }
	}
      temp = rp->head;
      while(temp != NULL)
	{
	  // printf("@@@@@@@@@@@@@@@@@@@@@@@@@@ temp is null in RR block time update\n");
	  if(temp->isBlocked == 1)
	    {
	      if ((temp->remainingBlockTime - sumRunTime) <= 0)
		{
		  temp->isBlocked = 0;
		  temp->remainingBlockTime = 200;
		}
	      else
		{
		  temp->remainingBlockTime -= sumRunTime;
		}
	    }
	  temp = temp->next;
	}
      //printf("*********************************************current time = %d\n", current_time);
      
      
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
