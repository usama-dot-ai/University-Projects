#ifndef STATS_H
#define STATS_H

#include "philosopher.h"

typedef struct {
    long meals_eaten[100];      // per philosopher
    long total_meals;
    double avg_wait_time_us;    // average waiting time per philosopher
    long max_wait_time_us;
    int deadlock_detected;
    long freeze_time_sec;
} SimulationStats;

void stats_init(SimulationStats *stats, int n);
void stats_record_meal(SimulationStats *stats, int phil_id);
void stats_record_wait_time(SimulationStats *stats, int phil_id, long wait_us);
void stats_compute_averages(SimulationStats *stats, int n);
void stats_print_summary(SimulationStats *stats, int n, const char *mode_name);
void stats_save_csv(SimulationStats *stats, int n, const char *filename);

#endif
