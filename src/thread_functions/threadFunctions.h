#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

#include "thread_operations/consumer.h"
#include "thread_operations/producer.h"
#include "thread_operations/deadlockTimer.h"

//////////////////////////////
//    PRODUCER FUNCTIONS    //
//////////////////////////////
#if PRODUCER_BOTH > 0
void *produce_both(void* arg);
#endif

#if PRODUCER_A > 0
void *produce_A(void* arg);
#endif

#if PRODUCER_B > 0
void *produce_B(void* arg);
#endif

//////////////////////////////
//    CONSUMER FUNCTIONS    //
//////////////////////////////

#if CONSUMER_BOTH > 0
void *consume_both();
#endif

#if CONSUMER_A > 0
void *consume_A(void* arg);
#endif

#if CONSUMER_B > 0
void *consume_B(void* arg);
#endif

//////////////////////////////////////////////////////////
//    MIXED FUNCTIONS WHICH BOTH PRODUCE AND CONSUME    //
////////////////////////////// ///////////////////////////
#if PRODUCER_A_CONSUMER_B > 0
void *produceA_consume_B(void* arg);
#endif

#if PRODUCER_B_CONSUMER_A > 0
void *produceB_consume_A(void* arg);
#endif

#endif