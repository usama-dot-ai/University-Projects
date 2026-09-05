#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

typedef enum {
    THINKING,
    HUNGRY,
    EATING
} PhilosopherState;

typedef struct {
    sem_t sem;
    int id;
} Chopstick;

typedef struct Philosopher {
    int id;
    int meals_eaten;
    long total_wait_time;      // microseconds spent hungry
    PhilosopherState state;
    pthread_t thread;
    Chopstick *left_chop;
    Chopstick *right_chop;
    void *(*run)(void*);       // function pointer: deadlock or safe algorithm
} Philosopher;

extern volatile bool simulation_running;

void init_philosophers(int n, Philosopher **phils, Chopstick **sticks, bool safe_mode);
void cleanup_philosophers(int n, Philosopher *phils, Chopstick *sticks);
void* deadlock_philosopher(void *arg);
void* safe_philosopher(void *arg);
void update_philosopher_state(Philosopher *p, PhilosopherState new_state);

#endif
