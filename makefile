CC=g++
CFLAGS=-g -Wall
OFLAGS=-O3
LFLAGS=-lm
FILES=main.cpp

simulation:
	$(CC) $(CFLAGS) $(OFLAGS) $(FILES) -o $@ $(LFLAGS)

clean:
	@rm -Rf simulation

