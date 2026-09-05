# Dining Philosophers

A C-based simulation of the classic **Dining Philosophers Problem**, developed for the **Operating Systems Lab at Air University Kharian**.

The project demonstrates **concurrency, synchronization, inter-process resource management, and deadlock prevention** using POSIX threads and semaphores.

## Overview

The simulation represents philosophers as concurrent threads competing for shared chopsticks. It implements two approaches:

* **Deadlock Mode** — demonstrates the classic circular-wait deadlock.
* **Safe Mode** — prevents deadlock by enforcing a resource hierarchy, requiring each philosopher to acquire the lower-numbered chopstick first.

Philosopher states are displayed in real time using terminal-based visualization:

* `THINKING`
* `HUNGRY`
* `EATING`

The program also records state changes and performance information in CSV format.

## Key Concepts

* POSIX Threads (`pthreads`)
* Semaphores (`sem_t`)
* Thread synchronization
* Shared resource management
* Deadlock and circular wait
* Coffman deadlock conditions
* Resource hierarchy
* Signal handling
* Performance measurement
* CSV logging

## Project Structure

```text
dining_philosophers/
├── src/
│   ├── main.c
│   ├── philosopher.c
│   ├── philosopher.h
│   ├── deadlock.c
│   ├── solution.c
│   ├── init.c
│   ├── display.c
│   ├── stats.c
│   └── logger.c
│
├── data/
├── Makefile
└── README.md
```

The implementation separates simulation control, philosopher logic, deadlock and safe algorithms, initialization, terminal display, statistics, and logging.

## Requirements

* Ubuntu Linux 22.04 / 24.04 LTS
* GCC
* POSIX Threads
* POSIX Semaphores
* GNU Make
* ANSI-compatible terminal

## Build & Run

```bash
make clean
make
```

### Safe Mode

```bash
./dining -n 5 -t 10 -m safe
```

### Deadlock Mode

```bash
./dining -n 5 -t 15 -m deadlock
```

### Benchmark Mode

```bash
./dining -n 5 -t 30 -m safe -b
```

Benchmark mode runs without the terminal display and generates CSV performance data.

## Deadlock Prevention

The deadlock-prone implementation acquires the left chopstick first and then attempts to acquire the right one. Under the appropriate conditions, this creates a **circular wait**, causing all philosophers to become blocked.

The safe implementation uses **resource hierarchy**:

```text
Acquire lower-numbered chopstick
            ↓
Acquire higher-numbered chopstick
            ↓
           Eat
            ↓
Release both chopsticks
```

By enforcing a global ordering for resource acquisition, the circular-wait condition is eliminated.

## Testing

The project was tested with different numbers of philosophers and execution modes. Tests covered:

* Safe-mode operation
* Deadlock demonstration
* 3 and 4 philosopher configurations
* Benchmark mode and CSV generation
* `Ctrl+C` signal handling
* Stress testing with 10 philosophers
* Single-philosopher edge case

All documented test cases passed successfully.

## Results

In a 30-second safe-mode test with five philosophers, the simulation recorded **94 total meals**, with an average waiting time of approximately **1.48 seconds**. The deadlock mode reached deadlock after approximately **5–8 seconds**, after which meal counts stopped increasing.

## Team

| Member                | Primary Contribution                                      |
| --------------------- | --------------------------------------------------------- |
| **Usama Arshad**      | Core engine, synchronization, and deadlock-free algorithm |
| **Ibtehaj Ali Mirza** | Visualization, CLI, user input, and signal handling       |
| **Asad Farooq**       | Statistics, CSV logging, and reporting                    |

## Course

**Operating Systems Lab**
**Air University Kharian**
**Project:** Dining Philosophers
**Date:** May 31, 2026
