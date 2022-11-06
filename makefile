CC=gcc
CFLAGS=-g -Wall
OFLAGS=-O3
LFLAGS=-lm
FILES=main.cpp

début:
	$(CC) $(CFLAGS) $(OFLAGS) $(FILES) -o $@ $(LFLAGS)

clean:
	@rm -Rf début

