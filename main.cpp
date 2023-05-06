// NOTATIONS :
// E_paire(r) = 4 * E_0 * [ (d/r)^12 - (d/r)^6 ] : énergie potentielle de paire de type Lennard-Jones de l'intéraction
// E_0  : profondeur du puit de potentiel (paramètre ajustable)
//    E_0 = 119,8 * k_b pour l'argon
// d    : distance d'annulation du potentiel (paramètre ajustable)
//    d = 3,405 pour l'argon
// r    : distance inter-atomique

// b_x  : dimension de la boîte de modélisation selon x
// b_y  : dimension de la boîte de modélisation selon y
// b_z  : dimension de la boîte de modélisation selon z

// r_c      : côté de la cellule
// r_cut    : rayon de coupure
// r_v      : rayon de verlet
// delta_r  : rayon de verlet - rayon de coupure
// r_max    : déplacement maximal

// f_i= - (dE_i(r)/dr(t+dt)) : force d’interaction
// sum_i_n(f_i)              : sommes des forces d'interaction
// i                         : interaction interatomique
// n                         : ensemble des interactions ayant lieux sur l'atome
// a_i = m/sum_i_n(f_i)      : accélération d'un atome induites par les n interactions
// p : nouvelle position d'un atome

// TROUVER LA POSITION : implémenter la méthode d'intégration de Verlet. Calcul de p(t+dt) avec p(t) et p(t-dt). Pour ne pas sauvegarder en mémoire la position précédente d'un atome : on calcule 2 demi pas de temps à la suite pour avoir un pas de temps.

// CHOISIR CONDITIONS AUX LIMITES : périodiques OU murs aux frontières.

// VERSIONS :
// Version 1 : pas de cellule
// Version 2 : décomposition du domaine en cellules
// Version 3 : cellules + rayon de Verlet pour ne pas avoir à construire une liste de voisin à chaque itération

// ENTREE ET SORTIE
// lecture et écriture de fichier type XYZ généré avec le logiciel atomsk

#include <iostream>
#include <cstddef>
#include <cstdio>
#include <random>
#include <mpi.h>
#include <x86intrin.h>
#include <time.h>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "Headers/types.h"
#include "Headers/constantes.h"
#include "SoA/particule.h"
#include "Headers/interaction.h"
#include "Headers/XYZ.h"
#include "Headers/remplissage_vecteurs.h"
#include "Headers/potentiel.h"
#include "Headers/cellules.h"

u32 N = 0;
u32 nb_iteration = 0;
u32 dt = 0;
u32 b_x = 0; // NEW
u32 b_y = 0; // NEW
u32 b_z = 0; // NEW

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    N = atoi(argv[1]);
    nb_iteration = atoi(argv[2]);
    dt = atoi(argv[3]);
    b_x = atoi(argv[4]); // NEW
    b_y = atoi(argv[5]); // NEW
    b_z = atoi(argv[6]); // NEW


    int rang, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    MPI_Comm_size(MPI_COMM_WORLD, &P);


    if (c_z < P) {
        std::cout << "Erreur : trop peu de processus disponibles. Arrêt du programme." << std::endl;
        return 1;
    }

    
    // Valeurs locales au processus
    int cellules_locales = c_z / P;

    if (rang == P-1) {
        cellules_locales += c_z % P;
    }
    



    if (rang == 0) {
        std::cout << "Ci-après un exemple de valeur de force répulsive : " << F_Lennard_Jones(0.1) << std::endl;
        std::cout << "Ci-après un exemple de valeur de force attractive : " << F_Lennard_Jones(1000) << std::endl;

        std::cout << std::endl << "Bienvenue dans l'exécution du programme développé par l'équipe 1 du M1 CHPS." << std::endl;
        std::cout << "Nous considérons comme paramètres :" << std::endl;
        std::cout << "     - " << N << " : nombre d'atomes('N')" << std::endl;
        std::cout << "     - " << nb_iteration << " : nombre d'itérations('nb_itérations'), i.e. autant d'applications de l'algorithme de Verlet-vitesses." << std::endl;
        std::cout << "     - " << dt << " fs : pas de temps ('dt') entre chaque itération."<< std::endl;
        std::cout << "Nous considérons les constantes physiques relatives au potentiel de Lennard-Jones :" << std::endl;
        std::cout << "     - " << E_0 << " u.Å²/fs² " << ": profondeur du puit de potentiel" << std::endl;
        std::cout << "     - " << d << " Å " << ": distance d'annulation du potentiel\n" << std::endl;
    }


    // Création des particules
    struct Particules_Para part;



    // Remplis les vecteurs avec les données de bases correspondantes pour chaque attribut.
    int n_local = remplissage_vecteurs_para(b_z, rang, P, part.ids, part.pos, part.vit, part.acc);



     
    
    std::string str_N = std::__cxx11::to_string(N);
    std::string str_rang = std::__cxx11::to_string(rang);
    ecrire_XYZ_Para_local("Sortie/simulation"+str_N+"_rang"+str_rang+".xyz", part.ids, part.pos, n_local);
    
    auto frontiere_type = Frontiere::Periodiques; //Frontiere::Murs


    
    // Cellules
    Cellules cellules;
    std::vector vec = cellules.vec;

    // Taille des cellules
    f64 tc_x = (f64)b_x / c_x;
    f64 tc_y = (f64)b_y / c_y;
    f64 tc_z = (f64)b_z / c_z;


    // Création des vecteurs
    for (int i = 0; i < cellules_locales+2; ++i) {
        std::vector<std::vector<std::vector<Particule_Cellule>>> v_z;
        vec.push_back(v_z);
        for (int j = 0; j < c_y+2; ++j) {
            std::vector<std::vector<Particule_Cellule>> v_y;
            vec[i].push_back(v_y);
            for (int k = 0; k < c_x+2; ++k) {
                std::vector<Particule_Cellule> v_x;
                vec[i][j].push_back(v_x);
            }      
        }
    }
    

    Particule_Cellule curr;
    int locales = c_z / P;
    
    // Stockage des particules dans les cellules
    for (int i = 0; i < n_local; ++i) {

        int ind_z = (part.pos.Z[i] / tc_z) - (locales*rang);
        int ind_y = (part.pos.Y[i] / tc_y);
        int ind_x = (part.pos.X[i] / tc_x);
        
        curr.id = part.ids[i];
        curr.X = part.pos.X[i];
        curr.Y = part.pos.Y[i];
        curr.Z = part.pos.Z[i];
        

        // Attention aux ghost cells   
        vec[ind_z+1][ind_y+1][ind_x+1].push_back(curr);
    }


    // Pour préparer les envois des cellules fantômes
    u32** comms = (u32**)malloc(sizeof(u32*)*4);
    for (int i = 0; i < 4; ++i) { // up_send - down_send - up_recv - down_recv
        comms[i] = (u32*)malloc(sizeof(u32) * ((c_y*c_x) +1));
        comms[i][0] = 0; // Index 0 = count
    }




    printf("Le rang %d a fini d'initialiser ses cellules locales.\n", rang);

    
    // Nouveaux types MPI pour les communications

        // Particule_Cellule
    
    MPI_Datatype particule_cellule_type;
    int blocs[2] = {3, 1};
    MPI_Datatype struct_types[2] = {MPI_DOUBLE, MPI_INT};
    MPI_Aint offset[4] = {offsetof(Particule_Cellule, X), 
                            offsetof(Particule_Cellule, Y), 
                            offsetof(Particule_Cellule, Z), 
                            offsetof(Particule_Cellule, id)};

    MPI_Type_create_struct(2, blocs, offset, struct_types, &particule_cellule_type);
    MPI_Type_commit(&particule_cellule_type);


    MPI_Datatype particule_comm_type;
    int blocs_comm[2] = {9, 1};
    MPI_Datatype struct_types_comm[2] = {MPI_DOUBLE, MPI_UNSIGNED};
    MPI_Aint offset_comm[10] = {offsetof(Particule_Comm, posX), 
                            offsetof(Particule_Comm, posY), 
                            offsetof(Particule_Comm, posZ), 

                            offsetof(Particule_Comm, vitX), 
                            offsetof(Particule_Comm, vitY), 
                            offsetof(Particule_Comm, vitZ), 

                            offsetof(Particule_Comm, accX), 
                            offsetof(Particule_Comm, accY), 
                            offsetof(Particule_Comm, accZ), 

                            offsetof(Particule_Comm, id)};

    MPI_Type_create_struct(2, blocs_comm, offset_comm, struct_types_comm, &particule_comm_type);
    MPI_Type_commit(&particule_comm_type);


    MPI_Datatype* types = (MPI_Datatype*)malloc(sizeof(MPI_Datatype)*2);
    types[0] = particule_cellule_type;
    types[1] = particule_comm_type;


    
    f64 r_cut_carre = 2.5*d*2.5*d; // Le potentiel est negligable r_cut = 2.5*d. // !NOUVEAU! ajout de 3 x multiplications




    MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "DEBUT ---------------" << std::endl;


    struct timespec start, end; // Début de la mesure de perf
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);



    
    for (u64 i = 1; i <= nb_iteration; i++) {

        VerletCellulesPara(rang, P, n_local, cellules_locales, vec, part, r_cut_carre, frontiere_type, types, comms);
        n_local = part.ids.size();

        
        std::string fichier_i = std::__cxx11::to_string(i);
        ecrire_XYZ_Para_local("Sortie/simulation"+str_N+"_iteration"+fichier_i+"_rang"+str_rang+".xyz", part.ids, part.pos, n_local);
        //std::cout << "Rang " << rang << " : Bonne création du fichier .xyz de la " << i << "-ème itération." << std::endl;
        //std::cout << "[" << i << "/" << nb_iteration << "] : Bonne écriture sur fichier des positions." << std::endl;
    }
    

    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end); // Fin de la mesure de perf


    f64 temps_s =  ((end.tv_sec - start.tv_sec) + ((f64)(end.tv_nsec - start.tv_nsec)/1e9));
    f64 capacite = (N*nb_iteration)/temps_s;

    MPI_Barrier(MPI_COMM_WORLD);

    printf("Rang %d Capacité : %.9f atome(s)/s\n", rang, capacite);
    
    
    MPI_Finalize();
    return 0;
}
