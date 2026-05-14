#ifndef CONSUMER_H
#define CONSUMER_H

#include "global_attributes/globalVariables.h"

void consume(const char* thread_name, char buffer_name, sem_t *Empty, pthread_mutex_t *Mutex, int Buffer[], int Buffer_size, int *out, int *y, sem_t *Full);

#endif