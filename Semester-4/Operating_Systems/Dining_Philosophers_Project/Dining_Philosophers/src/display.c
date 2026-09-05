#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_display() {
    printf("\033[2J\033[H");   // clear screen
}

void close_display() {
    printf("\033[?25h\n");
}

void update_display(Philosopher *phils, int n, long elapsed_sec) {
    printf("\033[H");   // move cursor home, but do NOT clear the whole screen
    
    // Header
    printf("=== DINING PHILOSOPHERS ===\n");
    
    // Legend – printed fresh every time to avoid remnants
    printf("Legend: \033[36mTHINKING\033[0m | \033[33mHUNGRY\033[0m | \033[32mEATING\033[0m\n");
    
    // Time line
    printf("Time: %ld sec\n", elapsed_sec);
    
    // Philosopher states
    for (int i = 0; i < n; i++) {
        const char *state_str;
        const char *color;
        if (phils[i].state == THINKING) {
            state_str = "THINKING";
            color = "\033[36m";
        } else if (phils[i].state == HUNGRY) {
            state_str = "HUNGRY";
            color = "\033[33m";
        } else {
            state_str = "EATING";
            color = "\033[32m";
        }
        printf("%sP%d: %-8s\033[0m (meals: %d)\n",
               color, phils[i].id, state_str, phils[i].meals_eaten);
    }
    fflush(stdout);
}

void display_legend(void) {
    // This function is no longer needed – legend is inside update_display.
    // Keep it empty or delete calls to it from main.c.
}

void display_comparison_table(int n, long deadlock_meals[], long safe_meals[],
                              double deadlock_wait[], double safe_wait[]) {
    printf("\n========== COMPARISON TABLE ==========\n");
    printf("Phil | Deadlock meals | Safe meals | Deadlock wait(us) | Safe wait(us)\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%-3d | %-15ld | %-10ld | %-17.2f | %.2f\n",
               i, deadlock_meals[i], safe_meals[i], deadlock_wait[i], safe_wait[i]);
    }
    printf("=======================================\n");
}
