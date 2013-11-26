 #include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////NODE  STRUCT CODE//////////////////////////////////////////////////////////////////
typedef struct Node 
{
  int procID;
  struct Node *next;
  int remainingBlockTime;
  int isBlocked;

  //for print proc
  int start;
  int end;
  int run;
  int blocks;
  double utilPercent;
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
  newProc->next = NULL;
  newProc->remainingBlockTime = 200;
  newProc->isBlocked = 0;

  //For print proc
  newProc->start = 0;
  newProc->end = 0;
  newProc->run = 0;
  newProc->blocks = 0;
  newProc->utilPercent = 0.0;
  

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
	      if(tempToDelete->next == NULL)//test if its the tail
		{
		  rp->tail = temp;
		}
	      temp->next = tempToDelete->next;
	      tempToDelete->next = NULL;
	      free(tempToDelete);
	      tempToDelete = NULL;
	      rp->size--;
	      return 0;
	    }
	  temp = temp->next;
	}

      return 1;
    }
}

Node * pullProc(RunningProcesses *rp, int procID)
{
 
  Node *temp = NULL;
  Node *tempToPull = NULL;
  if(rp->size != 0)
    {
      temp = rp->head;
    }
  else
    {
      DEBUGPRINTF("ERROR: Tried to pull Proc on an empty list\n");
      return NULL;
    }
  //if the proc to be removed is the head
  if (rp->size == 1)
    {
      rp->head = NULL;
      rp->tail = NULL;
      temp->next = NULL;
      rp->size--;
      return temp;
    }
  if(rp->head->procID ==procID)
    {
      rp->head = temp->next;
      temp->next = NULL;
      rp->size--;
      return temp;
    }
  //else check the LinkedList for the Node
  else
    {
      while(temp->next != NULL)
	{
	  if(temp->next->procID == procID)
	    {
	      tempToPull = temp->next;
	      if(tempToPull->next == NULL)//test if it is the tail
		{
		  rp->tail = temp;
		}
	      temp->next = tempToPull->next;
	      tempToPull->next = NULL;
	      rp->size--;
	      return tempToPull;
	    }
	  temp = temp->next;
	}

      return NULL;
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
      printf("Process ID:%d Block:%d\n",temp->procID,temp->isBlocked);
      temp = temp->next;
    }
  return 0;
}
//////////////////////////////////////////////END OF LINKED LIST STRUCT//////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
