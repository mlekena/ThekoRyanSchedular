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
  Node * lastToRun = NULL;
  Node * temp = rp->head;
  int block = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int current_time = 0;
  int finish = 0;
  int runProcReturn = 0;
  int sumRunTime = 0;
  int runTime = time_interval;

  //scheduling phase
  while(rp-> size != 0)
    {
      //reset
      block = 0;
      arrival = 0;
      proc_arrival = 0;
      finish = 0;
      runProcReturn = 0;
      sumRunTime = 0;
      runTime = time_interval;
      //loop through the array 
      while(currProc->isBlocked == 1 && currProc != lastToRun)
	{
	  if(currProc->next != NULL)
	    {
	      currProc = currProc->next;
	    }
	  else
	    {
	      currProc = rp->head;
	    }
	}
      
      //if a process that can be run is found run it till time_interval or its blocked or finishes.
      if(currProc->isBlocked == 0 )
	{
	  lastToRun = currProc;
	  while (sumRunTime != time_interval)
	    {
	      runProcReturn = run_proc(proc, currProc->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);
	     
	      
	      if (runProcReturn != -1)
		{
		  current_time += runProcReturn;
		  runTime -= runProcReturn;
		  sumRunTime += runProcReturn;//measures the time till currprocs turn is up
		}
	      //arrival found
	      if (arrival == 1)
		{
		  addProc(rp,proc_arrival);
		}
	      //block found
	      if(block == 1)
		{
		  currProc->isBlocked = 1;
		  break;
		}
	      //finish process
	      if (finish == 1)
		{
		  removeProc(rp,currProc->procID);
		  break;
		}
	      
	    }//end of turn while
	}//end of if
      //all announced processes are blocked so norun-check-arrival should be run
      else
	{
	  runProcReturn = proc_norun_check_arrival(proc, time_interval, current_time, &arrival, &proc_arrival);
	  current_time += runProcReturn;
	}
      temp = rp->head;
      while(temp != NULL)
	{
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
      printProcesses(rp);
      proc_print(proc);
      DEBUGPRINTF("*********************************************current time = %d\n", current_time);
    }
  
return 0;
}
