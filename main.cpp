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
#include "interaction.cpp"
#include "XYZ.cpp"
#include "voisinlist.cpp"

// Prototypes de fonctions
void remplissage_vecteurs(struct Vecteur_3D*, struct Vecteur_3D*, struct Vecteur_3D*);
void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit);
void accelerations_initiales_nulles(struct Vecteur_3D* acc);

int main() {
    /*
    printf("nombre d'atomes en interaction avec l'atome = %d atomes \n",N);
    printf("profondeur du puit de potentiel : E_0 = -%e J \n",E_0);
    printf("distance d'annulation du potentiel : d = %e A \n",d);
    printf("énergie potentielle pour un atome à 1,000 m : E_paire = %e J \n",E_paire);
    */

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

    // ? f64 r_max = 0;
    // ? f64 sum_r_max = 0;
    // fair la 1er liste des liste de voisin//

/*      
    for (u64 i = 0; i < nb_iteration; i++){
        Verlet(particules, dt, 2.5*d);          // Le potentiel est négligable r_cut = 2.5*d.
        //sum_r_max += _r_max;
        //if( sum_r_max > delta_r){ //Redéfinire la liste de voisin// }
        ecrireXYZ(positions, "Simulation/simulation"+str_N+".xyz");
    }
*/
      
    for (u64 i = 1; i < nb_iteration; i++) {
        Verlet(particules, dt, 2.5*d); // Le potentiel est negligable r_cut = 2.5*d.
        // ? sum_r_max += _r_max;
        // ? if( sum_r_max > delta_r){ //Redéfinire la liste de voisin// }
        std::string fichier_i = std::__cxx11::to_string(i);
        ecrireXYZ(positions, "Simulation/simulation"+str_N+"_iteration"+fichier_i+".xyz");
    }
      
    return 0;
}

void remplissage_vecteurs(struct Vecteur_3D* pos, struct Vecteur_3D* vit, struct Vecteur_3D* acc) { // Remplit les vecteurs de données
    // Peut être optimisé avec une seule boucle au lieu de 3

    std::string str_N = std::__cxx11::to_string(N);
    lireXYZ("source"+str_N+".xyz", pos);
    std::cout << "\nBonne lecture du fichier des positions.\n" << std::endl;

    generation_gaussienne_des_vitesses(vit);
    std::cout << "\nBonne affectation gaussienne des vitesses.\n" << std::endl;

    accelerations_initiales_nulles(acc);
    std::cout << "\nBonne affectation des valeurs nulles pour les accélérations.\n" << std::endl;
}

void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit){

    std::random_device rd; // Génération d'une graine.
    std::mt19937 gen(rd()); // Génération par "gen" d'un entier non signé par l'algo de Mersenne en fonction de la graine.
    std::normal_distribution<> d_v_x{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
    std::normal_distribution<> d_v_y{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
    std::normal_distribution<> d_v_z{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.

    f64 qdm_systeme_x = 0 ;
    f64 qdm_systeme_y = 0 ;
    f64 qdm_systeme_z = 0 ;

// Pour chaque appel de d_v_x(gen), d_v_y(gen) et d_v_z(gen) : transformation de l'entier généré par gen en un nouveau double aléatoire (gaussien) autour de 0.
// Calcul de la composante selon x, y et z du vecteur quantité de mouvement total du système.
    for(u64 i = 0; i < N; i++){
        vit->X[i] = d_v_x(gen);
        qdm_systeme_x = qdm_systeme_x + m*(vit->X[i]);
        vit->Y[i] = d_v_y(gen);
        qdm_systeme_y = qdm_systeme_y + m*(vit->Y[i]);
        vit->Z[i] = d_v_z(gen);
        qdm_systeme_z = qdm_systeme_z + m*(vit->Z[i]);
    }

    std::cout << "Quantité de mouvement du système selon x avant ajustement: " << qdm_systeme_x << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon y avant ajustement: " << qdm_systeme_y << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon z avant ajustement: " << qdm_systeme_z << "A/s" << std::endl;

// Ajustement de qdm_systeme à 0, ie m*vit->X[i] = m*vit->X[i] - qdm_systeme_x, ETC.
    for(u64 i = 0; i < N; i++){
        vit->X[i] -= qdm_systeme_x/N/m;
        vit->Y[i] -= qdm_systeme_y/N/m;
        vit->Z[i] -= qdm_systeme_z/N/m;
//        std::cout << &vit->X[i] << ": vit_Gauss_après_qdm_syst_nulle_X = " <<vit->X[i] << std::endl;
//        std::cout << &vit->Y[i] << ": vit_Gauss_après_qdm_syst_nulle_Y = " <<vit->Y[i] << std::endl;
//        std::cout << &vit->Z[i] << ": vit_Gauss_après_qdm_syst_nulle_Z = " <<vit->Z[i] << std::endl;
    }

// Qdm totale après ajustement
    f64 qdm_systeme_après_x = 0 ;
    f64 qdm_systeme_après_y = 0 ;
    f64 qdm_systeme_après_z = 0 ;
    for(u64 i = 0; i < N; i++){
        qdm_systeme_après_x += m*(vit->X[i]);
        qdm_systeme_après_y += m*(vit->Y[i]);
        qdm_systeme_après_z += m*(vit->Z[i]);
    }

    std::cout << "Quantité de mouvement du système selon x après ajustement: " << qdm_systeme_après_x << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon y après ajustement: " << qdm_systeme_après_y << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon z après ajustement: " << qdm_systeme_après_z << "A/s" << std::endl;

// Calcul de la température en fonction de la norme du vecteur vitesse.
    f64 temperature = 0;
    for(u64 i = 0; i < N; i++){
        temperature = temperature + 1/(3*k_b*N)*m*(pow(vit->X[i],2.0)+pow(vit->Y[i],2.0)+pow(vit->Z[i],2.0));
    }

// Calcul de la vitesse selon initialisation à temperature_cible;
    for(u64 i = 0; i < N; i++){
        vit->X[i] = (vit->X[i])*sqrt(temperature_cible/temperature);
//        std::cout << &vit->X[i] << ": vit_Gauss_après_ajustement_Tcible_X = " <<vit->X[i] << std::endl;
        vit->Y[i] = (vit->Y[i])*sqrt(temperature_cible/temperature);
//        std::cout << &vit->Y[i] << ": vit_Gauss_après_ajustement_Tcible_Y = " <<vit->Y[i] << std::endl;
        vit->Z[i] = (vit->Z[i])*sqrt(temperature_cible/temperature);
//        std::cout << &vit->Z[i] << ": vit_Gauss_après_ajustement_Tcible_Z = " <<vit->Z[i] << std::endl;
    }
}

void accelerations_initiales_nulles(struct Vecteur_3D* acc){
    for(u64 i = 0; i < N; i++){
        acc->X[i] = 0.0;
//      std::cout << &acc->X[i] << ": accX = " <<acc->X[i] << std::endl;
        acc->Y[i] = 0.0;
//      std::cout << &acc->Y[i] << ": accY = " <<acc->Y[i] << std::endl;
        acc->Z[i] = 0.0;
//      std::cout << &acc->Z[i] << ": accZ = " <<acc->Z[i] << std::endl;
    }

}
