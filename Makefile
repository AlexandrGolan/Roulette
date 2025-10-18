CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LIBS = -lncurses
TARGET = roulette
SOURCE = roulette.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

debug: $(SOURCE)
	$(CC) $(CFLAGS) -g -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: clean debug
