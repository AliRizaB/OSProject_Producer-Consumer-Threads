#include "global_attributes/globalVariables.h"
#include "thread_functions/threadFunctions.h"

void init_threads();
void buffer_check_produce();
void create_threads(int insert_point, const char *prefix, int id, void *(*function)(void *), pthread_t *thread);
void join_threads(int how_many, pthread_t *thread);
void send_time_out_signal(int how_many, sem_t *where);
void destroy_threads();

int global_name_i = 0;

int main(int argc, char *argv[])
{
    startTime = time(NULL);
    init_threads();
    buffer_check_produce();
    int PRODUCER_TOTAL = PRODUCER_BOTH + PRODUCER_A + PRODUCER_B;
    int CONSUMER_TOTAL = CONSUMER_BOTH + CONSUMER_A + CONSUMER_B;
    int MIXED_THREAD_TOTAL = PRODUCER_A_CONSUMER_B + PRODUCER_B_CONSUMER_A;

    pthread_t p[PRODUCER_TOTAL];
    pthread_t c[CONSUMER_TOTAL];
    pthread_t procon[MIXED_THREAD_TOTAL];

    // create_threads(how_many, function, type);

    // Creating Producer Threads
    for (int i = 0; i < PRODUCER_TOTAL; i++)
    {
#if PRODUCER_BOTH > 0
        // CREATING THREADS which produces to both A and B buffer
        if (i < PRODUCER_BOTH)
        {
            create_threads(i, "Pab", i, produce_both, p);
        }
#endif

#if PRODUCER_A > 0
#if PRODUCER_BOTH > 0
        else if (i < PRODUCER_BOTH + PRODUCER_A)
#else
        if (i < PRODUCER_A)
#endif
        {
            create_threads(i, "Pa", i, produce_A, p);
        }
#endif

#if PRODUCER_B > 0
#if (PRODUCER_BOTH > 0 || PRODUCER_A > 0)
        else
#else
        if (i < PRODUCER_B)
#endif
        {
            create_threads(i, "Pb", i, produce_B, p);
        }
#endif
    }

    // Creating Consumer Threads
    for (int i = 0; i < CONSUMER_TOTAL; i++)
    {
#if CONSUMER_BOTH > 0

        if (i < CONSUMER_BOTH)
        {
            create_threads(i, "Cab", i, consume_both, c);
        }
#endif

#if CONSUMER_A > 0
#if CONSUMER_BOTH > 0

        else if (i < CONSUMER_BOTH + CONSUMER_A)
#else

        if (i < CONSUMER_A)
#endif
        {
            create_threads(i, "Ca", i, consume_A, c);
        }
#endif

#if CONSUMER_B > 0
#if (CONSUMER_BOTH > 0 || CONSUMER_A > 0)

        else
#else

        if (i < CONSUMER_B)
#endif
        {
            create_threads(i, "Cb", i, consume_B, c);
        }
#endif
    }

    // Creating MIXED Threads
    for (int i = 0; i < MIXED_THREAD_TOTAL; i++)
    {
// CREATING THREADS which produces to A and consumes from B buffer
#if PRODUCER_A_CONSUMER_B > 0
        if (i < PRODUCER_A_CONSUMER_B)
        {
            create_threads(i, "PaCb", i, produceA_consume_B, procon);
        }
#endif
// CREATING THREADS which produces to B and consumes from A buffer
#if PRODUCER_B_CONSUMER_A > 0
#if PRODUCER_A_CONSUMER_B > 0
        else
        {
            create_threads(i, "PbCa", i, produceB_consume_A, procon);
        }
#else
        if (i < PRODUCER_B_CONSUMER_A)
        {
            create_threads(i, "PbCa", i, produceB_consume_A, procon);
        }
#endif
#endif
    }

    // Waiting the program to finish
    sleep(RUN_TIMER);

    // POSTING to all semaphores so if any thread is stuck in a sem_wait()
    // wakes up, checks the clock, sees time is up, and exits.
    printf("BROADCASTING EXIT SIGNALS...\n");
    send_time_out_signal(PRODUCER_TOTAL + MIXED_THREAD_TOTAL, &ASemEmpty);
    send_time_out_signal(PRODUCER_TOTAL + MIXED_THREAD_TOTAL, &BSemEmpty);
    send_time_out_signal(CONSUMER_TOTAL + MIXED_THREAD_TOTAL, &ASemFull);
    send_time_out_signal(CONSUMER_TOTAL + MIXED_THREAD_TOTAL, &BSemFull);

    // Joining everything
    join_threads(PRODUCER_TOTAL, p);
    join_threads(CONSUMER_TOTAL, c);
    join_threads(MIXED_THREAD_TOTAL, procon);

    destroy_threads();
    printf("TOTAL: Production for [Buffer A] was %d\n", total_production_A);
    printf("TOTAL: Consumption for [Buffer A] was %d\n\n", total_consumption_A);

    printf("TOTAL: Production [Buffer B] was %d\n", total_production_B);
    printf("TOTAL: Consumption [Buffer B] was %d\n\n", total_consumption_B);

    printf("TOTAL: Production was %d\n", total_production);
    printf("TOTAL: Consumption was %d\n", total_consumption);

    return 0;
}

void init_threads()
{
    pthread_mutex_init(&AMutexBuffer, NULL);
    pthread_mutex_init(&BMutexBuffer, NULL);

    // Ask if empty which starts with 0 and finished being empty at the Buffer Size
    sem_init(&ASemEmpty, 0, A_BUFFER_SIZE);
    sem_init(&BSemEmpty, 0, B_BUFFER_SIZE);

    // No elements in Buffer so initial and stop value is 0
    sem_init(&ASemFull, 0, 0);
    sem_init(&BSemFull, 0, 0);
}

// Producer Checks for Error Handling
void buffer_check_produce()
{
    // Check For Both need A and B
    if (PRODUCER_BOTH > 0 && (B_BUFFER_SIZE <= 0 || A_BUFFER_SIZE <= 0))
    {
        fprintf(stderr, "ERROR: You Are trying to create a PRODUCER FOR BOTH while either A or B buffer is not initalized\n");
        fprintf(stderr, "ERROR: Please check configParameters.h\n");
        exit(1);
    }

    // Check For A need A
    if (PRODUCER_A > 0 && (A_BUFFER_SIZE <= 0))
    {
        fprintf(stderr, "ERROR: You Are trying to create a PRODUCER FOR A while Buffer A not initalized or given a invalid value\n");
        fprintf(stderr, "ERROR: Please check configParameters.h\n");
        exit(1);
    }

    if (PRODUCER_B > 0 && (B_BUFFER_SIZE <= 0))
    {
        fprintf(stderr, "ERROR: You Are trying to create a PRODUCER FOR A while Buffer B not initalized or given a invalid value\n");
        fprintf(stderr, "ERROR: Please check configParameters.h\n");
        exit(1);
    }

    // Check For Both need A and B
    if (PRODUCER_A_CONSUMER_B > 0 && (B_BUFFER_SIZE <= 0 || A_BUFFER_SIZE <= 0))
    {
        fprintf(stderr, "ERROR: You Are trying to create a PRODUCER_A_CONSUMER_B while either A or B buffer is not initalized or has an invalid value\n");
        fprintf(stderr, "ERROR: Please check configParameters.h\n");
        exit(1);
    }

    // Check For Both need A and B
    if (PRODUCER_B_CONSUMER_A > 0 && (B_BUFFER_SIZE <= 0 || A_BUFFER_SIZE <= 0))
    {
        fprintf(stderr, "ERROR: You Are trying to create a PRODUCER_B_CONSUMER_A while either A or B buffer is not initalized or has an invalid value\n");
        fprintf(stderr, "ERROR: Please check configParameters.h\n");
        exit(1);
    }
}

void create_threads(int insert_point, const char *prefix, int id, void *(*function)(void *), pthread_t *thread)
{
    snprintf(thread_names[global_name_i], 16, "%s%d", prefix, id);

    if (pthread_create(&thread[insert_point], NULL, function, thread_names[global_name_i]) != 0)
    {
        perror("Failed to create Threads");
    }
    global_name_i++;
}

void join_threads(int how_many, pthread_t *thread)
{
    for (int i = 0; i < how_many; i++)
    {
        int status = pthread_join(thread[i], NULL);

        if (status != 0)
        {
            fprintf(stderr, "Failed to join Thread[%d]: %s\n", i, strerror(status));
        }
    }
}

void send_time_out_signal(int how_many, sem_t *where)
{
    for (int i = 0; i < how_many; i++)
    {
        sem_post(where);
    }
}

void destroy_threads()
{
    sem_destroy(&ASemEmpty);
    sem_destroy(&ASemFull);

    sem_destroy(&BSemEmpty);
    sem_destroy(&BSemFull);

    pthread_mutex_destroy(&BMutexBuffer);
    pthread_mutex_destroy(&AMutexBuffer);
}