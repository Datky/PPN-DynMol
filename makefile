CC=g++
CFLAGS=-g -Wall
OFLAGS=-O3
LFLAGS=-lm
FILES=main.cpp

simulation: main.o interaction.o potentiel.o XYZ.o remplissage_vecteurs.o constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)


%.o: %.cpp types.h constantes.h SoA/particule.h
	$(CC) -c -o $@ $< 

clean:
	@rm -Rf *.o Simulation
.PHONY: clean

clean_o:
	@rm -Rf *.o
.PHONY: clean_o

clean_Simulation:
	@rm -Rf Simulation

.PHONY: clean_Simulation
