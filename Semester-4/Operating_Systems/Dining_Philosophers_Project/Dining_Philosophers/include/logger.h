#ifndef LOGGER_H
#define LOGGER_H

#include "philosopher.h"

void log_open(const char *filename);
void log_close(void);
void log_state_change(int phil_id, PhilosopherState old_state, PhilosopherState new_state, long timestamp_us);
void log_deadlock_detected(long timestamp_us);

#endif
