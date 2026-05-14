#include "consumer.h"

void consume(const char *thread_name, char buffer_name, sem_t *Empty, pthread_mutex_t *Mutex, int Buffer[], int Buffer_size, int *out, int *y, sem_t *Full)
{
    // Check if the timer is already up before we even wait
    // to wake us up at the end, y should be -1.
    if (difftime(time(NULL), startTime) >= RUN_TIMER)
    {
        *y = -1; // Set the exit signal
        return;
    }

    // If Sem Empty is full then we wait to be emty
    printf("[%s] WAITING   ---- [Buffer %c] Consumer waiting...\n", thread_name, buffer_name);
    sem_wait(Full);

    // Re-check the time. If the Main thread Re awakes the after the producers are joined
    if (difftime(time(NULL), startTime) >= RUN_TIMER)
    {
        *y = -1;
        return;
    }

    printf("[%s] LOCKING   ---- [Buffer %c] Performing an action on a shared Buffer\n", thread_name, buffer_name);
    pthread_mutex_lock(Mutex);

    *y = Buffer[(*out)];
    (*out) = ((*out) + 1) % Buffer_size;
    if (buffer_name == 'A')
    {
        total_consumption_A++;
    }
    else
    {
        total_consumption_B++;
    }
    total_consumption++;

    printf("[%s] CONSUMING ---- [Buffer %c] Extracted value: %d\n", thread_name, buffer_name, *y);

    pthread_mutex_unlock(Mutex);
    printf("[%s] UNLOCKING ---- [Buffer %c] Exiting critical section (Mutex Unlock)\n", thread_name, buffer_name);

    sem_post(Empty);
    printf("[%s] SIGNAL    ---- [Buffer %c] Incremented SemEmpty (Slot now available for Producer)\n", thread_name, buffer_name);

    usleep(CONSUMING * 1000);
}
