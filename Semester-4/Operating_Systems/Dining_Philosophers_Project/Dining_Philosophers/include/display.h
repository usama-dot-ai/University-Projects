#ifndef DISPLAY_H
#define DISPLAY_H

#include "philosopher.h"

void init_display(void);
void close_display(void);
void update_display(Philosopher *phils, int n_philosophers, long elapsed_sec);
void display_legend(void);
void display_comparison_table(int n, 
                              long deadlock_meals[], long safe_meals[],
                              double deadlock_wait[], double safe_wait[]);

#endif
