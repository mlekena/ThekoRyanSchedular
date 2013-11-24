#ifndef RunningProcesses_H
#define RunningProcesses_H

// Macro.  if(1) disables printing, if(0) turns it on.
#define DEBUGPRINTF if(1){}else printf

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

// Create your RunningProcesses struct using the filename of a schedule.txt
RunningProcesses * createRunningProcesses();

//add a process to the list
int addProc(RunningProcesses * rpList, int procID);

//remove a node from the list
int removeFirstProc(RunningProcesses *rpList);
int removeProc(RunningProcesses * rp, int removeProcID);

//print the current list as it stands. If empty it prints list empty
int printProcesses(RunningProcesses * rp);

#endif // RunningProcesses_H
