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


#define N 1000
#define E_0 119.8*1.380649*pow(10,-23)
#define d 3.405

#include <cstdio>
#include <cmath>
#include <random>
#include "interaction.cpp"
#include "potentiel.cpp"
#include "XYZ.cpp"

#include "types.h"
#include "SoA/particule.h"


int main(){
      u32 nb_PasDeTemps =15;
      f64 dt = 0.1;
      f64 r_cut = 0.1;
      Particules at{};

      at.pos = lireXYZ("100.xyz");
      ecrireXYZ(at.pos, "100_simuation.xyz");
      u32 n = at.pos.x.size();

      for (u32 i = 0; i < n; i++){
            m = 5;
      
            at.vit->X[i] = rand();
            at.vit->Y[i] = rand();
            at.vit->Z[i] = rand();
      }
    
      for (u32 i = 0; i < nb_PasDeTemps; i++){
            Verlet(at, dt, r_cut, m);
            ecrireXYZ(at.pos, "100_simuation.xyz");
      }

      return 0;
}
