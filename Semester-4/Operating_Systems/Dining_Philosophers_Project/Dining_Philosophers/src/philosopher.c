#include "philosopher.h"
#include "logger.h"
#include <sys/time.h>

static long current_timestamp_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000L + tv.tv_usec;
}

void update_philosopher_state(Philosopher *p, PhilosopherState new_state) {
    long now = current_timestamp_us();
    log_state_change(p->id, p->state, new_state, now);
    p->state = new_state;
}
