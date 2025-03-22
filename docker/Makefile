CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGETS = os_simulator fast_os_simulation os_simulator_auto

all: $(TARGETS)

os_simulator: os_simulator.c
	$(CC) $(CFLAGS) -o $@ $<

fast_os_simulation: fast_os_simulation.c
	$(CC) $(CFLAGS) -o $@ $<

os_simulator_auto: os_simulator.c
	$(CC) $(CFLAGS) -o $@ $< -DAUTO_MODE

clean:
	rm -f $(TARGETS)

.PHONY: all clean