CC=mpic++
CFLAGS=-Wall -g #: non (pour performances)
OFLAGS=-O3
LFLAGS=-lm
FILES=main.cpp

Bin/simulation: Bin/main.o Bin/interaction.o Bin/potentiel.o Bin/XYZ.o Bin/remplissage_vecteurs.o Bin/constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)

Bin/main.o: main.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/interaction.o: Sources/interaction.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/potentiel.o: Sources/potentiel.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/XYZ.o: Sources/XYZ.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/remplissage_vecteurs.o: Sources/remplissage_vecteurs.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/constantes.o: Sources/constantes.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

# TEST 
Bin/TEST: Bin/Test.o Bin/interaction.o Bin/potentiel.o Bin/XYZ.o Bin/remplissage_vecteurs.o Bin/constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)
Bin/Test.o: Test.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

# Règle pour suppression de tous les fichiers créés.
clean:
	@rm -Rf Bin/*.o Bin/simulation Entree Sortie 
.PHONY: clean

# Règle pour suppression uniquement des ".o" et de l'exécutable.
clean_o:
	@rm -Rf Bin/*.o Bin/simulation 
clean_test:
	@rm -Rf Bin/*.o Bin/TEST Test/Data_force_Lennard_Jones.dat Test/TEST_cible.xyz
.PHONY: clean_o

.PHONY: clean_Sortie
