#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "globalInclude.h"
#include "configuration/configParameters.h"

// Mutex helpes cause threads try to access the
// buffer at the same time this makes
// it only one can access it to the buffer at a given time
extern pthread_mutex_t AMutexBuffer;
extern pthread_mutex_t BMutexBuffer;

// To save the Number if the Space is full or try to consume but the buffer is empty
// We use semaphones to make the program wait for these actions.
extern sem_t ASemEmpty;
extern sem_t ASemFull;

extern sem_t BSemEmpty;
extern sem_t BSemFull;

extern time_t startTime;

// Threads
extern char thread_names[TOTAL_THREADS][16];


extern int ABuffer[A_BUFFER_SIZE];
extern int BBuffer[B_BUFFER_SIZE];

extern int inA;
extern int outA;

extern int inB;
extern int outB;

extern int total_production_A;
extern int total_consumption_A;

extern int total_production_B;
extern int total_consumption_B;

extern int total_production;
extern int total_consumption;

#endif
