#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"
#include "RunningProcesses.h"
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////NODE  STRUCT CODE//////////////////////////////////////////////////////////////////
typedef struct Node 
{
int procID;
struct Node *next;
int remainingBlockTime;
int isBlocked;
}Node;

typedef struct RunningProcesses
{
  struct Node *head;
  struct Node *tail;
  int size;
}RunningProcesses;

RunningProcesses * createRunningProcesses()
{
  RunningProcesses * rp_list = malloc(sizeof(RunningProcesses));
  rp_list->head = NULL;
  rp_list->tail = NULL;
  rp_list->size = 0;
  return rp_list;
}

int addProc(RunningProcesses * rpList, int procID)
{
  //create the newProc node
  struct Node * newProc = malloc(sizeof(Node));
  newProc->procID = procID;
  newProc->next = 0;
  newProc->remainingBlockTime = 200;
  newProc->isBlocked = 0;

  //set the head and tail appropriately in RunningProcess
  if(rpList->head == NULL)
    {
      rpList->head = newProc;
      rpList->tail = newProc;
    }
  else
    {
      rpList->tail->next = newProc;
      rpList->tail = newProc;
    }
  rpList->size ++;
  return 1;

}

int removeFirstProc(RunningProcesses *rpList)
{
  if(rpList->size == 0)
    {
      return -1;
    }
  else if(rpList->size == 1)
    {
      free(rpList->head);
      rpList->head = NULL;
      rpList->tail = NULL;
      rpList->size --;
      return 1;
    }
  else
    {
      Node *temp = rpList->head;
      rpList->head = rpList->head->next;
      free(temp);
      rpList->size --;
      return 1;
    }
}

//returns 0 if it found the proc to delete and deleted it else it returns 1
int removeProc(RunningProcesses * rp, int removeProcID)
{
  Node *temp = NULL;
  Node *tempToDelete = NULL;
  if(rp->size != 0)
    {
      temp = rp->head;
    }
  else
    {
      DEBUGPRINTF("ERROR: Tried to removeProc on an empty list\n");
      return -1;
    }
  //if the proc to be removed is the head
  if(rp->head->procID == removeProcID)
    {
      removeFirstProc(rp);
      return 0;
    }
  //else check the LinkedList for the Node
  else
    {
      while(temp->next != NULL)
	{
	  if(temp->next->procID == removeProcID)
	    {
	      tempToDelete = temp->next;
	      temp->next = tempToDelete->next;
	      tempToDelete->next = NULL;
	      free(tempToDelete);
	      rp->size--;
	      return 0;
	    }
	  temp = temp->next;
	}

      return 1;
    }
}

int printProcesses(RunningProcesses * rp)
{
  Node * temp = NULL;
  if (rp->size != 0)
    {
      temp = rp->head;
    }
  else
    {
      printf("lIST IS EMPTY\n");
      return 0;
    }

  while(temp != NULL)
    {
      printf("Process ID:%d\n",temp->procID);
      temp = temp->next;
    }
  return 0;
}
//////////////////////////////////////////////END OF LINKED LIST STRUCT//////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


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

  int block = 0;
  int arrival = 0;
  int proc_arrival = 0;
  int current_time = 0;
  int finish = 0;
  int runProcReturn = 0;


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

  while(rp->size != 0)
    {
      //reset
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
      if (temp != NULL)
	{
	  runProcReturn = run_proc(proc, temp->procID, runTime, &block, &finish, current_time, &arrival, &proc_arrival);
	}
      else
	{
	  runProcReturn = proc_norun_check_arrival(proc, time_interval, current_time, &arrival, &proc_arrival);
	  totalIdle += runProcReturn;
	}
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
      
      if (rp->size != 0)
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
