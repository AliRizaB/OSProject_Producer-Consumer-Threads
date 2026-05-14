#include "globalVariables.h"

pthread_mutex_t AMutexBuffer;
pthread_mutex_t BMutexBuffer;

sem_t ASemEmpty;
sem_t ASemFull;

sem_t BSemEmpty;
sem_t BSemFull;

time_t startTime;

char thread_names[TOTAL_THREADS][16];

int ABuffer[A_BUFFER_SIZE];
int BBuffer[B_BUFFER_SIZE];

int inA = 0;
int outA = 0;

int inB = 0;
int outB = 0;

int total_production_A = 0;
int total_consumption_A = 0;

int total_production_B = 0;
int total_consumption_B = 0;

int total_production = 0;
int total_consumption = 0;