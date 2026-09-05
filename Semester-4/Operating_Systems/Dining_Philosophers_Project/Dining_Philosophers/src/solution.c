#include <stdio.h>
#include "philosopher.h"
#include "logger.h"
#include <stdlib.h>
#include <sys/time.h>

static long get_time_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000L + tv.tv_usec;
}

void* safe_philosopher(void *arg) {
    Philosopher *p = (Philosopher*) arg;
    long think_min = 100000, think_max = 500000;
    long eat_min = 200000, eat_max = 400000;
    
    // Resource hierarchy: always pick smaller ID first
    Chopstick *first, *second;
    if (p->left_chop->id < p->right_chop->id) {
        first = p->left_chop;
        second = p->right_chop;
    } else {
        first = p->right_chop;
        second = p->left_chop;
    }
    
    while (simulation_running) {
        update_philosopher_state(p, THINKING);
        usleep(think_min + rand() % (think_max - think_min));
        
        update_philosopher_state(p, HUNGRY);
        long hungry_start = get_time_us();
        
        sem_wait(&first->sem);
        sem_wait(&second->sem);
        
        long wait_time = get_time_us() - hungry_start;
        p->total_wait_time += wait_time;
        
        update_philosopher_state(p, EATING);
        p->meals_eaten++;
        usleep(eat_min + rand() % (eat_max - eat_min));
        
        sem_post(&second->sem);
        sem_post(&first->sem);
    }
    return NULL;
}
