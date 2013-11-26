#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"
#include "RunningProcesses.h"

typedef struct MultiLevelQueue
{
  RunningProcesses * theQueues[3]; 

}MultiLevelQueue;

//create a multiLevelQueue data structure
MultiLevelQueue * create_MLQ()
{
  MultiLevelQueue * retval = malloc(sizeof(MultiLevelQueue));
  int i = 0;
  for ( i = 0; i <3; i++)
    {
      retval->theQueues[i] = malloc(sizeof(RunningProcesses));
      retval->theQueues[i] = createRunningProcesses();
    }
  

  return retval;
}

//add method for the multiLevelQueue
int addMLQProc(Node * proc, int level, MultiLevelQueue * MLQ)
{
  if(MLQ->theQueues[level]->size == 0)
    {
      MLQ->theQueues[level]->head = proc;
      MLQ->theQueues[level]->tail = proc;
      MLQ->theQueues[level]->tail->next = NULL;
      MLQ->theQueues[level]->size++;
    }
  else
    {
      MLQ->theQueues[level]->tail->next = proc;
      MLQ->theQueues[level]->tail = proc;
      proc->next = NULL;
      MLQ->theQueues[level]->size++;
    }

  return 0; 
}

//reduces a given process's priority
int reducePriority(int from, int procID, MultiLevelQueue * MLQ)
{
  if(from >= 0 && from <=2)
    {  
      if (from == 2)
	{
	  Node * temp = pullProc(MLQ->theQueues[from], procID);
	  addMLQProc(temp, from, MLQ);
	}
      else
	{
	  Node * temp = pullProc(MLQ->theQueues[from], procID);
	  addMLQProc(temp, from + 1, MLQ);
	}
    }
  else
    {
      printf("Invalid level number given to reducePriority method in MLQ.c: %d", from);
      return 1;
    }
  return 0;
}

//check if all the queues are empty
int isEmpty(MultiLevelQueue * MLQ)
{
  if(MLQ->theQueues[0]->size == 0 && MLQ->theQueues[1]->size == 0 && MLQ->theQueues[2]->size == 0 )
    {
      return 1;
    }
  return 0;
}

//used to check if queue has any prgrams not blocked
int hasRunnableProcess(RunningProcesses * rp)//check if the queue has any unblocked processes
{
  
  if (rp->size == 0)
    {
      return 0;
    }
  else
    {
      Node * temp = rp->head;;
      while(temp != NULL)
	{
	  if (temp->isBlocked == 0)
	    {
	      return 1;
	    }
	  temp = temp->next;
	}
      return 0;
    }
}

int printMLQ(MultiLevelQueue *MLQ)
{
  int i = 0;
  for(i = 0; i < 3; i++)
    {
      printf("*************\nLevel %d: ", i);
      printProcesses(MLQ->theQueues[i]);
      
    }
  return 0;
}

//runs through all the queues and updates the remaining block time 
int updateBlockTimes(MultiLevelQueue *MLQ, int time)
{
  Node *temp = MLQ->theQueues[0]->head;
  if(isEmpty(MLQ) != 1)
    {
	
      if(MLQ->theQueues[0]->size != 0)//if the level is not empty
	{
	  while(temp != NULL)//run through level
	    {
	      if(temp->isBlocked == 1)// if proc is blocked
		{
		  if((temp->remainingBlockTime - time) <= 0)    
		    {
		      temp->isBlocked = 0;
		      temp->remainingBlockTime = 200;
		    }
		  else
		    {
		      temp->remainingBlockTime -= time;
		    }
		}
	      temp = temp->next;
	    }
	}
      return 0;
	
    }
  return 1;
}

//runs the Mulitlevelqueue scheduling algorithm
int runMLQ(Processes * proc, int time_interval)
{
  MultiLevelQueue * MLQ = create_MLQ();
  Node * firstNd = malloc(sizeof(Node));
  firstNd->next = NULL;
  firstNd->procID = 0;
  firstNd->remainingBlockTime = 200;
  firstNd->isBlocked = 0;
  addMLQProc(firstNd, 0, MLQ);

  int block = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int current_time = 0;
  int finish = 0;
  int runProcReturn = 0;
  int runTime = time_interval;
  int noProcRun = 0;
  Node *temp = NULL;
  int fromLevel = 0;
  int timeQuanta = 0;

  Node *arriveNode = NULL;

  //used to record the final output data
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
  while( isEmpty(MLQ) != 1)
    {
      
      block = 0;
      arrival = 0;
      proc_arrival = 0;
      finish = 0;
      runProcReturn = 0;
      runTime = time_interval;
      noProcRun = 0;
      fromLevel = 0;
      timeQuanta = time_interval;


      if(MLQ->theQueues[0]->size > 0)//check to see if first level has processes
	{
	  temp = MLQ->theQueues[0]->head;
	  while(temp != NULL)
	    {
	      if(temp->isBlocked == 1 && temp->remainingBlockTime < runTime)
		{
		  runTime = temp->remainingBlockTime;
		}
	      temp = temp->next;
	    }
	}

      if (hasRunnableProcess(MLQ->theQueues[0]) == 1)//if the top queu has any programs that can even run
	{
	  //run on queue 0
	  temp = MLQ->theQueues[0]->head;
	  while(temp != NULL)
	    {
	      if(temp->isBlocked == 0)
		{
		  break;
		}
	      
	      temp = temp->next;
	    }
	  runProcReturn = run_proc(proc, temp->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);//run the proc
	  fromLevel = 0;
	  
	}
      else if (MLQ->theQueues[1]->size > 0)
	{
	  if(runTime == time_interval)
	    {
	      runTime = time_interval * 2;
	    }
	  temp = MLQ->theQueues[1]->head;
	  runProcReturn = run_proc(proc, temp->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);//run the proc
	  fromLevel = 1;
	  //run on queue 1
	}
      else if (MLQ->theQueues[2]->size > 0)
	{
	  if(runTime == time_interval)
	    {
	      runTime = time_interval * 4;
	    }
	  temp = MLQ->theQueues[2]->head;
	  runProcReturn = run_proc(proc, temp->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);//run the proc
	  fromLevel = 2;
	  //run on queue 2
	}
      else
	{
	  //run norun proc check
	  runProcReturn = proc_norun_check_arrival(proc, runTime, current_time, &arrival, &proc_arrival);//run the proc
	  totalIdle += runProcReturn;
	  noProcRun = 1;
	}

      //increment current time
      if (runProcReturn != -1)
	{
	  current_time += runProcReturn;
	}
      else
	{
	  printf("ERROR: Run_Proc returned -1\n");
	  exit(1);
	}

      //check cases
      if(noProcRun == 0)
	{
	  //arrival found
	  if (arrival == 1)
	    {
	      addProc(MLQ->theQueues[0],proc_arrival);
	      temp = pullProc(MLQ->theQueues[fromLevel], temp->procID);
	      addMLQProc(temp, 0, MLQ);
	      startArray[proc_arrival] = current_time;
	    }
	  //block found
	  else if(block == 1)
	    {
	      temp->isBlocked = 1;
	      temp = pullProc(MLQ->theQueues[fromLevel], temp->procID);
	      addMLQProc(temp, 0, MLQ);
	      blocksArray[temp->procID]++;
	    }
	  //finish process
	  else if (finish == 1)
	    {
	      endArray[temp->procID] = current_time;
	      upArray[temp->procID] = runArray[temp->procID] / endArray[temp->procID];
	      runArray[temp->procID] = endArray[temp->procID] - startArray[temp->procID];
	      removeProc(MLQ->theQueues[fromLevel],temp->procID);
	    }
	  else if(runProcReturn < 100)
	    {
	      temp = pullProc(MLQ->theQueues[fromLevel], temp->procID);
	      addMLQProc(temp, 0, MLQ);
	    }
	  else
	    {
	      reducePriority(fromLevel, temp->procID, MLQ);
	    }
      
	}
      else//ran proc_norun_check_arrival
	{
	  //arrival found
	  if (arrival == 1)
	    {
	      arriveNode = malloc(sizeof(Node));
	      arriveNode->next = NULL;
	      arriveNode->procID = proc_arrival;
	      arriveNode->remainingBlockTime = 200;
	      arriveNode->isBlocked = 0;
	      addMLQProc(arriveNode, 0, MLQ);
	    }
	}
      updateBlockTimes(MLQ, runProcReturn);
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
