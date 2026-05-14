#include "deadlockTimer.h"

void *deadlock_timer(void *arg)
{
    sleep(DEADLOCK_TIMEOUT);  // Wait
    printf("DEADLOCK DETECTED: Timed out waiting for A! Exiting...\n");
    exit(1);
    return NULL;
}