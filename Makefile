# Compiler and flags
CC = g++
CFLAGS = -Wall -O2
LIBS = -lSDL_bgi -lSDL2

# Default target builds both executables
all: field_lines dipole charge relax

relax: relax.cpp
	$(CC) $(CFLAGS) relax.cpp -o relax $(LIBS)

field_lines: field_lines.cpp
	$(CC) $(CFLAGS) field_lines.cpp -o field_lines $(LIBS)

dipole: dipole.cpp
	$(CC) $(CFLAGS) dipole.cpp -o dipole $(LIBS)

charge: charge.cpp
	$(CC) $(CFLAGS) charge.cpp -o charge $(LIBS)

clean:
	rm -f field_lines dipole charge relax

