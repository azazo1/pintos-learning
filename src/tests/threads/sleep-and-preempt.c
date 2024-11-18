#include <stdio.h>
#include "threads/thread.h"
#include "devices/timer.h"

void pri_low(void) {
    printf("priority-low-start-looping.\n");
    int64_t initial_ticks = timer_ticks();
    while (timer_ticks() <= initial_ticks + 200) {
    }
    printf("priority-low-end-looping.\n");
}

void pri_high(void) {
    printf("priority-high-start-sleeping.\n");
    timer_sleep(100);
    printf("priority-high-wakeup.\n");
}

void test_sleep_and_preempt(void) {
    thread_create("priority-low", PRI_MIN + 1, pri_low, NULL);
    thread_create("priority-high", PRI_MIN + 2, pri_high, NULL);

    timer_sleep(300); // sleep enough time.
}
