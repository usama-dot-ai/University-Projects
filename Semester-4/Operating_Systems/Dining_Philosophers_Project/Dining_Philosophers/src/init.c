#include "philosopher.h"
#include <stdlib.h>

void init_philosophers(int n, Philosopher **phils_out, Chopstick **sticks_out, bool safe_mode) {
    Philosopher *phils = malloc(n * sizeof(Philosopher));
    Chopstick *sticks = malloc(n * sizeof(Chopstick));
    
    for (int i = 0; i < n; i++) {
        sem_init(&sticks[i].sem, 0, 1);
        sticks[i].id = i;
    }
    
    for (int i = 0; i < n; i++) {
        phils[i].id = i;
        phils[i].meals_eaten = 0;
        phils[i].total_wait_time = 0;
        phils[i].state = THINKING;
        phils[i].left_chop = &sticks[i];
        phils[i].right_chop = &sticks[(i+1) % n];
        if (safe_mode)
            phils[i].run = safe_philosopher;
        else
            phils[i].run = deadlock_philosopher;
    }
    
    *phils_out = phils;
    *sticks_out = sticks;
}

void cleanup_philosophers(int n, Philosopher *phils, Chopstick *sticks) {
    for (int i = 0; i < n; i++) {
        sem_destroy(&sticks[i].sem);
    }
    free(phils);
    free(sticks);
}
