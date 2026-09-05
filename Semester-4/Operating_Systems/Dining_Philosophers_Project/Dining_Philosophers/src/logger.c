#include "logger.h"
#include <stdio.h>

static FILE *log_file = NULL;

void log_open(const char *filename) {
    log_file = fopen(filename, "w");
    if (log_file)
        fprintf(log_file, "timestamp_us,philosopher_id,old_state,new_state\n");
}

void log_close(void) {
    if (log_file) fclose(log_file);
}

static const char* state_to_string(PhilosopherState s) {
    switch(s) {
        case THINKING: return "THINKING";
        case HUNGRY:   return "HUNGRY";
        case EATING:   return "EATING";
        default:       return "UNKNOWN";
    }
}

void log_state_change(int phil_id, PhilosopherState old_state, PhilosopherState new_state, long timestamp_us) {
    if (log_file)
        fprintf(log_file, "%ld,%d,%s,%s\n", timestamp_us, phil_id,
                state_to_string(old_state), state_to_string(new_state));
}

void log_deadlock_detected(long timestamp_us) {
    if (log_file)
        fprintf(log_file, "%ld,-1,DEADLOCK_DETECTED,DEADLOCK\n", timestamp_us);
}
