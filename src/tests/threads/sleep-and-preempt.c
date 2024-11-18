#include <stdio.h>
#include "threads/thread.h"
#include "devices/timer.h"

void pri_median(void) {
    printf("median-start-looping.\n");
    int64_t initial_ticks = timer_ticks();
    while (timer_ticks() <= initial_ticks + 250) {
    }
    printf("median-end-looping.\n");
}

void pri_low(void) {
    printf("low-start-sleeping.\n");
    timer_sleep(100);
    printf("low-wakeup.\n");
}

void pri_high(void) {
    printf("high-start-sleeping.\n");
    timer_sleep(50);
    printf("high-wakeup.\n");
}

void test_sleep_and_preempt(void) {
    thread_create("low", PRI_MIN + 1, pri_low, NULL);
    timer_sleep(10); // 确保 low 得到执行而不是连开始运行的机会都没有, 这里的优先级是正常的排序, 与苏醒抢占部分无关.
    thread_create("median", PRI_MIN + 2, pri_median, NULL);
    thread_create("high", PRI_MIN + 3, pri_high, NULL);

    timer_sleep(300); // sleep enough time.
}