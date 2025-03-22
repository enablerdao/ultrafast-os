CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = fast_os_simulation
SRC = fast_os_simulation.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean