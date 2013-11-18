#ifndef FCFS_H
#define FCFS_H

// Macro.  if(1) disables printing, if(0) turns it on.
#define DEBUGPRINTF if(1){}else printf


// Create your Processes struct using the filename of a schedule.txt
int runFCFS(Processes *proc, int *time_interval);



#endif // PROCESSES_H
