# Compiler and flags
CC = g++
CFLAGS = -Wall -O2
LIBS = -lSDL_bgi -lSDL2

# Default target builds both executables
all: field_lines dipole

field_lines: field_lines.cpp
	$(CC) $(CFLAGS) field_lines.cpp -o field_lines $(LIBS)

dipole: dipole.cpp
	$(CC) $(CFLAGS) dipole.cpp -o dipole $(LIBS)

clean:
	rm -f field_lines dipole

