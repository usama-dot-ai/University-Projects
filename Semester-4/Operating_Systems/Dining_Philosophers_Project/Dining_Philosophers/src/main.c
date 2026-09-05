#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>
#include "philosopher.h"
#include "display.h"
#include "stats.h"
#include "logger.h"

volatile bool simulation_running = true;

void signal_handler(int sig) {
    (void)sig;
    simulation_running = false;
}

void run_simulation(int n, int runtime_sec, bool safe_mode, bool benchmark_mode) {
    Philosopher *phils = NULL;
    Chopstick *sticks = NULL;
    SimulationStats stats;
    stats_init(&stats, n);
    
    init_philosophers(n, &phils, &sticks, safe_mode);
    
    if (!benchmark_mode) {
        init_display();
    }
    
    for (int i = 0; i < n; i++)
        pthread_create(&phils[i].thread, NULL, phils[i].run, &phils[i]);
    
    long start_sec = time(NULL);
    while (simulation_running && (time(NULL) - start_sec) < runtime_sec) {
        long elapsed = time(NULL) - start_sec;
        if (!benchmark_mode) {
            update_display(phils, n, elapsed);
            usleep(500000);
        } else {
            usleep(100000);
        }
    }
    
    simulation_running = false;
    for (int i = 0; i < n; i++)
        pthread_join(phils[i].thread, NULL);
    
    // Collect statistics
    stats.total_meals = 0;
    stats.max_wait_time_us = 0;
    double wait_sum = 0.0;
    for (int i = 0; i < n; i++) {
        stats.meals_eaten[i] = phils[i].meals_eaten;
        stats.total_meals += phils[i].meals_eaten;
        if (phils[i].total_wait_time > stats.max_wait_time_us)
            stats.max_wait_time_us = phils[i].total_wait_time;
        wait_sum += phils[i].total_wait_time;
    }
    stats.avg_wait_time_us = wait_sum / n;
    
    if (!benchmark_mode) {
        stats_print_summary(&stats, n, safe_mode ? "SAFE" : "DEADLOCK");
        close_display();
    }
    
    char filename[256];
    sprintf(filename, "data/%s_%d_%d.csv", safe_mode ? "safe" : "deadlock", n, runtime_sec);
    stats_save_csv(&stats, n, filename);
    
    cleanup_philosophers(n, phils, sticks);
}

int main(int argc, char *argv[]) {
    int n = 5, t = 30;
    bool safe_mode = true, benchmark_mode = false;
    int opt;
    while ((opt = getopt(argc, argv, "n:t:m:b")) != -1) {
        switch (opt) {
            case 'n': n = atoi(optarg); break;
            case 't': t = atoi(optarg); break;
            case 'm': safe_mode = (strcmp(optarg, "safe") == 0); break;
            case 'b': benchmark_mode = true; break;
            default:
                fprintf(stderr, "Usage: %s [-n num] [-t sec] [-m deadlock|safe] [-b]\n", argv[0]);
                exit(1);
        }
    }
    signal(SIGINT, signal_handler);
    system("mkdir -p data");
    char logname[256];
    sprintf(logname, "data/sim_%ld.log", time(NULL));
    log_open(logname);
    run_simulation(n, t, safe_mode, benchmark_mode);
    log_close();
    return 0;
}
