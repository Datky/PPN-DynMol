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
#include <cstdio>
#include <random>
#include <x86intrin.h>
#include "Headers/types.h"
#include "Headers/constantes.h"
#include "SoA/particule.h"
#include "Headers/interaction.h"
#include "Headers/XYZ.h"
#include "Headers/remplissage_vecteurs.h"
#include "Headers/potentiel.h"
#include "Headers/cellules.h"

int main() {
    std::cout << "Ci-après un exemple de valeur de force répulsive : " << F_Lennard_Jones(0.1) << std::endl;
    std::cout << "Ci-après un exemple de valeur de force attractive : " << F_Lennard_Jones(1000) << std::endl;

    std::cout << std::endl << "Bienvenue dans l'exécution du programme développé par l'équipe 1 du M1 CHPS." << std::endl;
    std::cout << "Nous considérons comme paramètres :" << std::endl;
    std::cout << "     - " << N << " : nombre d'atomes('N')" << std::endl;
    std::cout << "     - " << nb_iteration << " : nombre d'itérations('nb_itérations'), i.e. autant d'applications de l'algorithme de Verlet-vitesses." << std::endl;
    std::cout << "     - " << dt << " fs : pas de temps ('dt') entre chaque itération."<< std::endl;
    std::cout << "Nous considérons les constantes physiques relatives au potentiel de Lennard-Jones :" << std::endl;
    std::cout << "     - " << E_0 << " u.Å²/fs² " << ": profondeur du puit de potentiel" << std::endl;
    std::cout << "     - " << d << " Å " << ": distance d'annulation du potentiel" << std::endl;

    // Création et allocation des particules
    struct Particules particules;

    particules.pos = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
    particules.vit = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
    particules.acc = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));

    particules.pos->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    particules.pos->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    particules.pos->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

    particules.vit->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    particules.vit->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    particules.vit->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

    particules.acc->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    particules.acc->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    particules.acc->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

    struct Vecteur_3D *__restrict positions = particules.pos;
    struct Vecteur_3D *__restrict vitesses = particules.vit;
    struct Vecteur_3D *__restrict accelerations = particules.acc;

    remplissage_vecteurs(positions, vitesses, accelerations); // Remplis les vecteurs avec les données de bases correspondantes pour chaque attribut.      

    std::string str_N = std::__cxx11::to_string(N);
    ecrireXYZ(positions, "Sortie/simulation"+str_N+".xyz");

    auto frontiere_type = Frontiere::Periodiques; //Frontiere::Murs




    /*
    // Cellules
    Cellules cellules;
    std::vector vec = cellules.vec;


    // Calcul du nombre total de cellules + ghost cell
    int nombre_cellules_total = ((c_x+2) * (c_y+2) * (c_z+2));
    // Création des vecteurs
    for (int i = 0; i < nombre_cellules_total; ++i) {
        std::vector<u32> v;
        vec.push_back(v);
    }

    // Insertion des particules dans les cellules
    int min = (c_x+2) * (c_y+2); // À cause des ghost cell
    for (int i = 0; i < N; ++i) {
        f64 z = positions->Z[i];
    }
    */


    // Cellules
    Cellules cellules;
    std::vector vec = cellules.vec;

    // Taille des cellules
    f64 tc_x = b_x / c_x;
    f64 tc_y = b_y / c_y;
    f64 tc_z = b_z / c_z;


    // Création des vecteurs
    for (int i = 0; i < c_z+2; ++i) {
        std::vector<std::vector<std::vector<u32>>> v_z;
        vec.push_back(v_z); 
        for (int j = 0; j < c_y+2; ++j) {
            std::vector<std::vector<u32>> v_y;
            vec[i].push_back(v_y);
            for (int k = 0; k < c_x+2; ++k) {
                std::vector<u32> v_x;
                vec[i][j].push_back(v_x);
            }      
        }
    }


    // Stockage des particules dans les cellules
    for (u32 i = 0; i < N; ++i) {
        int ind_z = positions->Z[i] / tc_z;
        int ind_y = positions->Y[i] / tc_y;
        int ind_x = positions->X[i] / tc_x;

        // Attention aux ghost cells
        vec[ind_z+1][ind_y+1][ind_x+1].push_back(i);
    }

    std::cout << "Positions de base bien enregistrées dans les cellules.\n" << std::endl;


    f64 r_cut_carre = 2.5*d*2.5*d; // Le potentiel est negligable r_cut = 2.5*d. // !NOUVEAU! ajout de 3 x multiplications
    u64 debut = __rdtsc(); // Début de la mesure de perf

    
    for (u64 i = 1; i <= nb_iteration; i++) {
        //Verlet(particules, r_cut_carre, frontiere_type); // !NOUVEAU! économie de nb_iteration x multiplications
        VerletCellules(vec, particules, r_cut_carre, frontiere_type);

        std::string fichier_i = std::__cxx11::to_string(i);
        ecrireXYZ(positions, "Sortie/simulation"+str_N+"_iteration"+fichier_i+".xyz");
        std::cout << "Bonne création du fichier .xyz de la " << i << "-ème itération." << std::endl;
        std::cout << "[" << i << "/" << nb_iteration << "] : Bonne écriture sur fichier des positions." << std::endl;

    }

    u64 fin = __rdtsc(); // Fin de la mesure de perf
    u64 total = fin-debut;
    std::cout << "\nLa simulation s'est exécutée en " << total << " cycles CPU (Moyenne : " << total/nb_iteration << ")." << std::endl;
      
    return 0;
}
