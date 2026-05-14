#ifndef CONFIGPARAMETERS_H
#define CONFIGPARAMETERS_H

// TIMER CONFIGS
#define RUN_TIMER 60 // Second
#define PRODUCING 10 // MiliSecond ( 1 second = 1 000 )
#define CONSUMING 10 // MiliSecond ( 1 second = 1 000 )
/*
    FOR EXPERIMENT 5:
    And so on for the production time for the Mixed Function Threads
    will be same as CONSUMING time as defined here.
*/

// BUFFER CONFIG
#define A_BUFFER_SIZE 5
#define B_BUFFER_SIZE 0

// Deadlock Initiate
// Want Deadlock = 1
// No Deadlock = 0
// Work for only when CONSUMER_BOTH is working like in the 3. experiment
#define DEADLOCK_TIMEOUT 2 // Second
#define DEADLOCK 0

// THREAD CONFIGURATION

// PRODUCER CONFIG
// Both meaning it will produce both A and B buffer
#define PRODUCER_BOTH 0

#define PRODUCER_A 2
#define PRODUCER_B 0

// CONSUMER CONFIG
// Both meaning it will consume both A and B buffer
#define CONSUMER_BOTH 0

#define CONSUMER_A 2
#define CONSUMER_B 0

// MIXED THREADS WHICH BOTH CONSUME AND PRODUCE
#define PRODUCER_A_CONSUMER_B 0
#define PRODUCER_B_CONSUMER_A 0

#define TOTAL_THREADS (PRODUCER_BOTH + PRODUCER_A + PRODUCER_B + \
                       CONSUMER_BOTH + CONSUMER_A + CONSUMER_B + \
                       PRODUCER_A_CONSUMER_B + PRODUCER_B_CONSUMER_A)

#endif