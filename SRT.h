#ifndef SRT_H
#define SRT_H

// Macro.  if(1) disables printing, if(0) turns it on.
#define DEBUGPRINTF if(0){}else printf

int runSRT(Processes *proc, int time_interval);

#endif //SRT_H
