CC=mpic++
CFLAGS=-Wall #-g #: non (pour performances)
OFLAGS=-O1 #-O1 #-03 pr V10
LFLAGS=-lm
# FILES=main.cpp

#all: Bin/simulation Bin/simulation_omp Bin/simulation_mpi Bin/simulation_CL
all: Bin/simulation Bin/simulation_omp

Bin/simulation: Bin/main.o Bin/interaction.o Bin/potentiel.o Bin/XYZ.o Bin/remplissage_vecteurs.o Bin/constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)

Bin/simulation_CL: Bin/main_CL.o Bin/interaction.o Bin/potentiel.o Bin/XYZ.o Bin/remplissage_vecteurs.o Bin/constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)

Bin/simulation_omp: Bin/main_omp.o Bin/interaction_omp.o Bin/potentiel.o Bin/XYZ.o Bin/remplissage_vecteurs.o Bin/constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS) -fopenmp

#Bin/simulation_mpi: Bin/main_mpi.o Bin/interaction_mpi.o Bin/potentiel.o Bin/XYZ_mpi.o Bin/remplissage_vecteurs_mpi.o Bin/constantes.o
#	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)

Bin/main.o: main.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $<

Bin/main_CL.o: main_CL.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $<

Bin/main_omp.o: main_omp.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< -fopenmp

#Bin/main_mpi.o: main_mpi.cpp Headers/types.h Headers/constantes.h SoA/particule_mpi.h
#	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $<

Bin/interaction.o: Sources/interaction.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

#Bin/interaction_mpi.o: Sources/interaction_mpi.cpp Headers/types.h Headers/constantes.h SoA/particule_mpi.h
#	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/interaction_omp.o: Sources/interaction_omp.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $<  -fopenmp

Bin/potentiel.o: Sources/potentiel.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/XYZ.o: Sources/XYZ.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

#Bin/XYZ_mpi.o: Sources/XYZ_mpi.cpp Headers/types.h Headers/constantes.h SoA/particule_mpi.h
#	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/remplissage_vecteurs.o: Sources/remplissage_vecteurs.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

#Bin/remplissage_vecteurs_mpi.o: Sources/remplissage_vecteurs_mpi.cpp Headers/types.h Headers/constantes.h SoA/particule_mpi.h
#	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

Bin/constantes.o: Sources/constantes.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

# TEST 
Bin/TEST: Bin/Test.o Bin/interaction.o Bin/potentiel.o Bin/XYZ.o Bin/remplissage_vecteurs.o Bin/constantes.o
	$(CC) $(CFLAGS) $(OFLAGS) $^ -o $@ $(LFLAGS)
Bin/Test.o: Test.cpp Headers/types.h Headers/constantes.h SoA/particule.h
	$(CC) $(CFLAGS) $(OFLAGS) -c -o $@ $< 

# Règle pour suppression de tous les fichiers créés.
clean:
	@rm -Rf Bin/*.o Bin/simulation Entree Sortie Sortie_omp Sortie_mpi Bin Terminal Resultats Courbes valgrind* callgrind* Test/diff*
.PHONY: clean

# Règle pour suppression uniquement des ".o" et de l'exécutable.
clean_o:
	@rm -Rf Bin/*.o Bin/simulation* 
clean_test:
	@rm -Rf Bin/*.o Bin/TEST Test/Data_force_Lennard_Jones.dat Test/TEST_cible.xyz
.PHONY: clean_o

.PHONY: clean_Sortie
