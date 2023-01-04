CC=g++
CFLAGS=-Wall #-g : non (pour performances)
OFLAGS=-O3
LFLAGS=-lm
FILES=main.cpp

simulation: main.o interaction.o potentiel.o XYZ.o remplissage_vecteurs.o constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)

%.o: %.cpp types.h constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

# Règle pour suppression de tous les fichiers créés.
clean:
	@rm -Rf *.o simulation Entree Sortie
.PHONY: clean

# Règle pour suppression uniquement des ".o" et de l'exécutable.
clean_o:
	@rm -Rf *.o simulation 
.PHONY: clean_o

.PHONY: clean_Sortie
