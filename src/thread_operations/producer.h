#ifndef PRODUCER_H
#define PRODUCER_H

#include "global_attributes/globalVariables.h"

void produce(const char* thread_name, char buffer_name, sem_t *Empty, pthread_mutex_t *Mutex, int Buffer[], int buffer_size, int *in, int *x, sem_t *Full, int wait_time);



#endif