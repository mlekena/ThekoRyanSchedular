#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "processes.h"

int runFCFS(Processes *proc, int *time_interval)
{
  int i =0;
  int zero = 0;
  DEBUGPRINTF("running FCFS");
  for(i=0; i < proc->size; i++)
    {
      while(proc->array[i].finish == 0)
	{
	  run_proc(proc, i, *time_interval, proc[i].block, proc[i].finish, current_time, &zero, &zero);
	}
      
    }
  

  return 0;
}
