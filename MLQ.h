#ifndef MLQ_H
#define MLQ_H

// Macro.  if(1) disables printing, if(0) turns it on.
#define DEBUGPRINTF if(1){}else printf

typedef struct MultiLevelQueue MultiLevelQueue;

//runs the MultilevelQueue schedule
int runMLQ();

#endif
