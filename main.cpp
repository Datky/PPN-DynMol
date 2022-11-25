// NOTATIONS (A VALIDER ENSEMBLE) :
// E_paire(r) = 4 * E_0 * [ (d/r)^12 - (d/r)^6 ] : énergie potentielle de paire de type Lennard-Jones de l'intéraction
// E_0  : profondeur du puit de potentiel (paramètre ajustable)
//    E_0 = 119,8 * k_b pour l'argon
// k_b  : constante de Boltzman
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
#include <cmath>
#include <random>
#include "types.h"
#include "constantes.h"
#include "SoA/particule.h"

// Protoypes de fonctions
int verify_particule_allocation(Particules*);


int main(){
      double r = 1; // (à revoir) valeur arbitraire juste pour entrer la formule
      double E_paire = 4*E_0*(pow(d/r,12.0)-pow(d/r,6.0));

      printf("nombre d'atomes en interaction avec l'atome = %d atomes \n",N);
      printf("profondeur du puit de potentiel : E_0 = -%e J \n",E_0);
      printf("distance d'annulation du potentiel : d = %e A \n",d);
      printf("énergie potentielle pour un atome à 1,000 m : E_paire = %e J \n",E_paire);



      struct Particules particules;
      struct Particules *ptc_ptr = &particules;

      // Vérification de la bonne création des particules
      int memory_error = verify_particule_allocation(ptc_ptr);

      if (memory_error > 0) {
            std::cout << "\nErreur de création des particules, fin du programme.\n" << std::endl;
            return 1;
      }

      std::cout << "\nBonne création des particules.\n" << std::endl;

}


int verify_particule_allocation(Particules* part) { // Vérifie la bonne allocation des particules
      int memory_error = 0;

    if (part->pos == nullptr) {
        std::cout << "Erreur d'allocation des positions." << std::endl;
        memory_error++;
    }
    if (part->vit == nullptr) {
        std::cout << "Erreur d'allocation des vitesses." << std::endl;
        memory_error++;
      }
    if (part->pos == nullptr) {
        std::cout << "Erreur d'allocation des accélérations." << std::endl;
        memory_error++;
      }

      return memory_error;
}