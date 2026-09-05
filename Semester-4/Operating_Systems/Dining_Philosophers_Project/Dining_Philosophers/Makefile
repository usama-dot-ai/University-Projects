CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude -g
LDFLAGS = -pthread
TARGET = dining
SRCDIR = src
OBJDIR = obj
DATADIR = data

SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/philosopher.c \
          $(SRCDIR)/deadlock.c \
          $(SRCDIR)/solution.c \
          $(SRCDIR)/display.c \
          $(SRCDIR)/stats.c \
          $(SRCDIR)/logger.c \
          $(SRCDIR)/init.c

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(DATADIR)/*.csv $(DATADIR)/*.log

run-deadlock: $(TARGET)
	./$(TARGET) -n 5 -t 20 -m deadlock

run-safe: $(TARGET)
	./$(TARGET) -n 5 -t 20 -m safe

benchmark: $(TARGET)
	@echo "Benchmarking deadlock mode..."
	./$(TARGET) -n 5 -t 30 -m deadlock -b || true
	@echo "Benchmarking safe mode..."
	./$(TARGET) -n 5 -t 30 -m safe -b

.PHONY: all clean run-deadlock run-safe benchmark
