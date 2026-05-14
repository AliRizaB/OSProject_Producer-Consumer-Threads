#include "threadFunctions.h"

//////////////////////////////
//    PRODUCER FUNCTIONS    //
//////////////////////////////

#if PRODUCER_BOTH > 0
void *produce_both(void *arg)
{
    char *thread_name = (char *)arg;

    // Produce
    int xA = 0;
    int xB = 0;
    while (difftime(time(NULL), startTime) < RUN_TIMER)
    {
        produce(thread_name, 'A', &ASemEmpty, &AMutexBuffer, ABuffer, A_BUFFER_SIZE, &inA, &xA, &ASemFull, PRODUCING);

        produce(thread_name, 'B', &BSemEmpty, &BMutexBuffer, BBuffer, B_BUFFER_SIZE, &inB, &xB, &BSemFull, PRODUCING);
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif

#if PRODUCER_A > 0
void *produce_A(void *arg)
{
    char *thread_name = (char *)arg;
    int xA = 0;
    while (difftime(time(NULL), startTime) < RUN_TIMER)
    {
        produce(thread_name, 'A', &ASemEmpty, &AMutexBuffer, ABuffer, A_BUFFER_SIZE, &inA, &xA, &ASemFull, PRODUCING);
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif

#if PRODUCER_B > 0
void *produce_B(void *arg)
{
    char *thread_name = (char *)arg;
    int xB = 0;
    while (difftime(time(NULL), startTime) < RUN_TIMER)
    {
        produce(thread_name, 'B', &BSemEmpty, &BMutexBuffer, BBuffer, B_BUFFER_SIZE, &inB, &xB, &BSemFull, PRODUCING);
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif

//////////////////////////////
//    CONSUMER FUNCTIONS    //
//////////////////////////////

#if CONSUMER_BOTH > 0
void Deadlock(void *arg);
void Normal_Work(void *arg);

void *consume_both(void *arg)
{
    if (DEADLOCK == 1)
        Deadlock(arg);

    else
        Normal_Work(arg);

    return NULL;
}
#endif

#if CONSUMER_A > 0
void *consume_A(void *arg)
{
    char *thread_name = (char *)arg;
    int y = -2;

    while (1)
    {
        // Check Buffer A
        consume(thread_name, 'A', &ASemEmpty, &AMutexBuffer, ABuffer, A_BUFFER_SIZE, &outA, &y, &ASemFull);

        if (y == -1)
            break;
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif

#if CONSUMER_B > 0
void *consume_B(void *arg)
{
    char *thread_name = (char *)arg;
    int y = -2;

    while (1)
    {
        // Check Buffer B
        consume(thread_name, 'B', &BSemEmpty, &BMutexBuffer, BBuffer, B_BUFFER_SIZE, &outB, &y, &BSemFull);

        if (y == -1)
            break;
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif
#if CONSUMER_BOTH > 0
void Normal_Work(void *arg)
{
    char *thread_name = (char *)arg;
    int y = -2;
    while (1)
    {
        // Check Buffer A
        consume(thread_name, 'A', &ASemEmpty, &AMutexBuffer, ABuffer, A_BUFFER_SIZE, &outA, &y, &ASemFull);

        if (y == -1)
            break;

        // Check Buffer B
        consume(thread_name, 'B', &BSemEmpty, &BMutexBuffer, BBuffer, B_BUFFER_SIZE, &outB, &y, &BSemFull);

        if (y == -1)
            break;
    }

    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);
}

void Deadlock(void *arg)
{
    char *thread_name = (char *)arg;
    int y;
    while (1)
    {
        pthread_t deadlockTimer;

        // Grab Buffer B
        sem_wait(&BSemFull);
        pthread_mutex_lock(&BMutexBuffer);

        // DEADLOCK
        printf("[%s] DEBUG: I have B, waiting for A...\n", thread_name);

        // Start deadlock_timer BEFORE the dangerous wait
        pthread_create(&deadlockTimer, NULL, deadlock_timer, NULL);

        // FORCED BLOCK: This thread will wait here
        // if the Producer is also waiting for B.
        sem_wait(&ASemFull); // If this blocks forever, timer fires

        // Made it through — cancel the timer
        pthread_cancel(deadlockTimer);
        pthread_join(deadlockTimer, NULL);

        
        pthread_mutex_lock(&AMutexBuffer);

        y = ABuffer[outA];
        outA = (outA + 1) % A_BUFFER_SIZE;
        y = BBuffer[outB];
        outB = (outB + 1) % B_BUFFER_SIZE;

        printf("[%s] CONSUMED ---- from both A and B Buffer\n", thread_name);

        pthread_mutex_unlock(&AMutexBuffer);
        pthread_mutex_unlock(&BMutexBuffer);
        sem_post(&ASemEmpty);
        sem_post(&BSemEmpty);

        total_consumption++;
    }

    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);
}
#endif

//////////////////////////////////////////////////////////
//    MIXED FUNCTIONS WHICH BOTH PRODUCE AND CONSUME    //
////////////////////////////// ///////////////////////////

#if PRODUCER_A_CONSUMER_B > 0
void *produceA_consume_B(void *arg)
{
    char *thread_name = (char *)arg;
    int xA = 0;
    int yB = 0;
    while (difftime(time(NULL), startTime) < RUN_TIMER)
    {
        produce(thread_name, 'A', &ASemEmpty, &AMutexBuffer, ABuffer, A_BUFFER_SIZE, &inA, &xA, &ASemFull, CONSUMING);

        consume(thread_name, 'B', &BSemEmpty, &BMutexBuffer, BBuffer, B_BUFFER_SIZE, &outB, &yB, &BSemFull);
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif

#if PRODUCER_B_CONSUMER_A > 0
void *produceB_consume_A(void *arg)
{
    char *thread_name = (char *)arg;
    int xB = 0;
    int yA = 0;
    while (difftime(time(NULL), startTime) < RUN_TIMER)
    {
        produce(thread_name, 'B', &BSemEmpty, &BMutexBuffer, BBuffer, B_BUFFER_SIZE, &inB, &xB, &BSemFull, CONSUMING);

        consume(thread_name, 'A', &ASemEmpty, &AMutexBuffer, ABuffer, A_BUFFER_SIZE, &outA, &yA, &ASemFull);
    }
    printf("*** [%s] FINISHING ---- FINISHING SIGNAL RECEIVED FINISHING THE OPERATION\n", thread_name);

    return NULL;
}
#endif
