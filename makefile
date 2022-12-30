CC=g++
CFLAGS=-g -Wall
OFLAGS=-O3
LFLAGS=-lm
FILES=main.cpp

simulation: main.o interaction.o potentiel.o XYZ.o remplissage_vecteurs.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)


%.o: %.c types.h constantes.h SoA/particule.h
	$(CC) -c -o $@ $< 

clean:
	@rm -Rf *.o simulation
.PHONY: clean
