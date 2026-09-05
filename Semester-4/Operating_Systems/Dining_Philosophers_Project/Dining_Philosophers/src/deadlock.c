#include "philosopher.h"
#include "logger.h"
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static long get_time_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000L + tv.tv_usec;
}

void* deadlock_philosopher(void *arg) {
    Philosopher *p = (Philosopher*) arg;
    
    // Stagger start slightly
    usleep(50000 * p->id);
    
    while (simulation_running) {
        // Think for a fixed time (0.2 seconds)
        update_philosopher_state(p, THINKING);
        usleep(200000);
        
        // Become hungry
        update_philosopher_state(p, HUNGRY);
        long hungry_start = get_time_us();
        
        // Pick up left chopstick
        sem_wait(&p->left_chop->sem);
        
        // CRITICAL: Wait long enough for EVERY philosopher to pick up left chopstick
        // With 5 philosophers, 0.5 seconds is plenty.
        usleep(500000);  // 0.5 second delay – this guarantees all hold left
        
        // Now try to pick up right chopstick
        // At this point, every philosopher holds left and waits for right.
        // No one will ever get right because it's held by the neighbor.
        sem_wait(&p->right_chop->sem);   // This line will block forever – DEADLOCK
        
        // The following code is never reached after deadlock occurs
        long wait_time = get_time_us() - hungry_start;
        p->total_wait_time += wait_time;
        
        update_philosopher_state(p, EATING);
        p->meals_eaten++;
        usleep(300000);
        
        sem_post(&p->right_chop->sem);
        sem_post(&p->left_chop->sem);
    }
    return NULL;
}
