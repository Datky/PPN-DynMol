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
#include "types.h"
#include "constantes.h"
#include "SoA/particule.h"
#include "interaction.h"
#include "XYZ.h"
#include "remplissage_vecteurs.h"

int main() {
//NEW
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
    ecrireXYZ(positions, "Simulation/simulation"+str_N+".xyz");

    auto frontiere_type = Frontiere::Periodiques; //Frontiere::Murs
    // ? f64 r_max = 0;
    // ? f64 sum_r_max = 0;
    // fair la 1er liste des liste de voisin//

/*      
    for (u64 i = 0; i < nb_iteration; i++){
        Verlet(particules, 2.5*d, frontiere_type);          // Le potentiel est négligable r_cut = 2.5*d.
        //sum_r_max += _r_max;
        //if( sum_r_max > delta_r){ //Redéfinire la liste de voisin// }
        ecrireXYZ(positions, "Simulation/simulation"+str_N+".xyz");
    }
*/
      
    for (u64 i = 1; i <= nb_iteration; i++) {
        Verlet(particules, 2.5*d, frontiere_type); // Le potentiel est negligable r_cut = 2.5*d.
        // ? sum_r_max += _r_max;
        // ? if( sum_r_max > delta_r){ //Redéfinire la liste de voisin// }
        std::string fichier_i = std::__cxx11::to_string(i);
        ecrireXYZ(positions, "Simulation/simulation"+str_N+"_iteration"+fichier_i+".xyz");
        std::cout << "Bonne création du fichier .xyz de la " << i << "-ème itération." << std::endl;
//        std::cout << particules.pos->X[222] << std::endl; //NEW
    }
      
    return 0;
}
