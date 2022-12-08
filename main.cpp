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
#include "XYZ.cpp"

// Protoypes de fonctions
void remplissage_vecteurs(struct Vecteur_3D*, struct Vecteur_3D*, struct Vecteur_3D*);
void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit);
void accelerations_initiales_nulles(struct Vecteur_3D* acc);

int main() {
      double r = 1; // (à revoir) valeur arbitraire juste pour entrer la formule
      double E_paire = 4*E_0*(pow(d/r,12.0)-pow(d/r,6.0));

      printf("nombre d'atomes en interaction avec l'atome = %d atomes \n",N);
      printf("profondeur du puit de potentiel : E_0 = -%e J \n",E_0);
      printf("distance d'annulation du potentiel : d = %e A \n",d);
      printf("énergie potentielle pour un atome à 1,000 m : E_paire = %e J \n",E_paire);


 

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




      return 0;
}

// !!! A REMPLACER PAR LECTURE DU FICHIER .XYZ
void remplissage_vecteurs(struct Vecteur_3D* pos, struct Vecteur_3D* vit, struct Vecteur_3D* acc) { // Remplit les vecteurs de données

  // Peut être optimisé avec une seule boucle au lieu de 3


  lireXYZ("source10000.xyz", pos);

  std::cout << "\nBonne lecture du fichier des positions.\n" << std::endl;

  generation_gaussienne_des_vitesses(vit);

  std::cout << "\nBonne affectation gaussienne des vitesses.\n" << std::endl;

  accelerations_initiales_nulles(acc);

  std::cout << "\nBonne affectation des accélérations.\n" << std::endl;
}

void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit){

  std::random_device rd; // Génération d'une graine.
  std::mt19937 gen(rd()); // Génération par "gen" d'un entier non signé par l'algo de Mersenne en fonction de la graine.
  std::normal_distribution<> d_v_x{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
  std::normal_distribution<> d_v_y{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
  std::normal_distribution<> d_v_z{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.

// Pour chaque appel de d_v_x(gen), d_v_y(gen) et d_v_z(gen) : transformation de l'entier généré par gen en un nouveau double aléatoire (gaussien) autour de 0.
  for(u64 i = 0; i < N; i++){
    vit->X[i] = d_v_x(gen);
    std::cout << &vit->X[i] << ": vit_Gauss_X = " <<vit->X[i] << std::endl;
    vit->Y[i] = d_v_y(gen);
    std::cout << &vit->Y[i] << ": vit_Gauss_Y = " <<vit->Y[i] << std::endl;
    vit->Z[i] = d_v_z(gen);
    std::cout << &vit->Z[i] << ": vit_Gauss_Z = " <<vit->Z[i] << std::endl;
  }
}

void accelerations_initiales_nulles(struct Vecteur_3D* acc){
  for(u64 i = 0; i < N; i++){
    acc->X[i] = 0.0;
    std::cout << &acc->X[i] << ": accX = " <<acc->X[i] << std::endl;
    acc->Y[i] = 0.0;
    std::cout << &acc->Y[i] << ": accY = " <<acc->Y[i] << std::endl;
    acc->Z[i] = 0.0;
    std::cout << &acc->Z[i] << ": accZ = " <<acc->Z[i] << std::endl;
  }

}
