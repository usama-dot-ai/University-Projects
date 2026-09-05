#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

void stats_init(SimulationStats *stats, int n) {
    for (int i = 0; i < n && i < 100; i++)
        stats->meals_eaten[i] = 0;
    stats->total_meals = 0;
    stats->avg_wait_time_us = 0.0;
    stats->max_wait_time_us = 0;
    stats->deadlock_detected = 0;
    stats->freeze_time_sec = 0;
}

void stats_record_meal(SimulationStats *stats, int phil_id) {
    if (phil_id >= 0 && phil_id < 100)
        stats->meals_eaten[phil_id]++;
}

void stats_record_wait_time(SimulationStats *stats, int phil_id, long wait_us) {
    (void)stats; (void)phil_id; (void)wait_us;  // suppress warnings
}

void stats_compute_averages(SimulationStats *stats, int n) {
    long sum = 0;
    for (int i = 0; i < n; i++)
        sum += stats->meals_eaten[i];
    stats->total_meals = sum;
}

void stats_print_summary(SimulationStats *stats, int n, const char *mode_name) {
    (void)n;  // suppress warning – placed at the top
    printf("\n======= %s MODE SUMMARY =======\n", mode_name);
    printf("Total meals: %ld\n", stats->total_meals);
    printf("Avg wait time: %.2f µs\n", stats->avg_wait_time_us);
    printf("Max wait time: %ld µs\n", stats->max_wait_time_us);
    if (stats->deadlock_detected)
        printf("Deadlock detected after %ld seconds\n", stats->freeze_time_sec);
    else
        printf("No deadlock detected.\n");
    printf("================================\n");
}

void stats_save_csv(SimulationStats *stats, int n, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "philosopher_id,meals_eaten\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d,%ld\n", i, stats->meals_eaten[i]);
    fprintf(f, "# total_meals,%ld\n", stats->total_meals);
    fprintf(f, "# avg_wait_us,%.2f\n", stats->avg_wait_time_us);
    fprintf(f, "# max_wait_us,%ld\n", stats->max_wait_time_us);
    fclose(f);
}
