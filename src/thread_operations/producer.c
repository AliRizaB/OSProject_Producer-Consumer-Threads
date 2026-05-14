#include "producer.h"

void produce(const char *thread_name, char buffer_name, sem_t *Empty, pthread_mutex_t *Mutex, int Buffer[], int buffer_size, int *in, int *x, sem_t *Full, int wait_time)
{
    // If Sem Empty is full then we wait to be empty
    printf("[%s] WAITING   ---- [Buffer %c] Producer waiting for an item to be insert...\n", thread_name, buffer_name);
    sem_wait(Empty);

    printf("[%s] LOCKING   ---- [Buffer %c] Performing an action on a shared Buffer\n", thread_name, buffer_name);
    pthread_mutex_lock(Mutex);
    // Adding to BUFFER

    Buffer[*in] = *x;
    *in = (*in + 1) % buffer_size;
    if (buffer_name == 'A')
    {
        total_production_A++;
    }
    else
    {
        total_production_B++;
    }
    total_production++;
    printf("[%s] PRODUCING ---- [Buffer %c] Inserted value: %d\n", thread_name, buffer_name, *x);

    printf("[%s] UNLOCKING ---- [Buffer %c] Action is performed on the shared buffer\n", thread_name, buffer_name);
    pthread_mutex_unlock(Mutex);
    // After adding an element to the buffer we post it to Full to acknowledge it
    sem_post(Full);
    printf("[%s] SIGNAL    ---- [Buffer %c] Incremented SemFull (It is now can be consumed by the Consumer)\n", thread_name, buffer_name);
    usleep(wait_time * 1000);

    (*x)++;
}