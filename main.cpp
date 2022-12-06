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

// n_a      : nombre d'atomes
// n_c      : nombre de cellules pour un boîte carrée

// r_c      : côté de la cellule
// r_cut    : rayon de coupure
// r_v      : rayon de verlet
// delta_r  : rayon de verlet - rayon de coupure
// r_max    : déplacement maximal ???

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

#include <cmath>
//#include <random>
#include "types.h"
#include <cstdlib> // vérifier si nécessaire
#include <iostream>
#include <ctime>

#define n_a 10000 // Nombre d'atomes
#define E_0 119.8*1.380649*pow(10,-23)
#define d 3.405
#define b_x 5.000
#define b_y 5.000
#define b_z 1.000
#define h 0.001 // pas de temps
#define tps 1.0 // temps total

int main(){

// Déclaration du tableau de masses
double masse[n_a];

// Déclaration du tableau des énergies
double energie[n_a];

// Déclaration de la classe cinematique comprenant des tableaux selon x et y
// SoA : Structure of Array
class cinematique{
public:
    double x[n_a];
    double y[n_a];
};

// Instanciations avec les objets position, vitesse, acceleration
cinematique position{};
cinematique vitesse{};
cinematique acceleration{};
cinematique force{};

// Pour chaque atome : initialisation des masses, énergies, positions, vitesses et accélérations, (et forces...)
for (int i=0;i<n_a;i++){
    masse[i] = 1;
    energie[i] = 0;
    position.x[i] = 1;
    position.y[i] = 1;
    vitesse.x[i] = 0;
    vitesse.y[i] = 0;
    acceleration.x[i] = 0;
    acceleration.y[i] = 0;
    force.x[i] = 1;
    force.y[i] = 1;
}

// Algorithme de Verlet : boucle interne selon les pas de temps
double duree = 0;
while (duree<tps){
// Algorithme de Verlet : boucle interne selon les atomes
    for (int i=0;i<n_a;i++){
// 1) Position après h
        position.x[i] = position.x[i] + vitesse.x[i] + acceleration.x[i]*h*h/2;
        position.y[i] = position.y[i] + vitesse.y[i] + acceleration.y[i]*h*h/2;
// 2) Vitesse après h/2
        vitesse.x[i] = vitesse.x[i] + acceleration.x[i]*h;
        vitesse.y[i] = vitesse.y[i] + acceleration.y[i]*h;
// 3) Force après h
// !!! todo : double E_paire = 4*E_0*(pow(d/r,12.0)-pow(d/r,6.0));
// Avant cela : force affectée constante et égale à 1
// 4) Accélération après h
        acceleration.x[i] = force.x[i]/masse[i];
        acceleration.y[i] = force.y[i]/masse[i];
// 5) Vitesse après h
        vitesse.x[i] = vitesse.x[i] + acceleration.x[i]*h/2;
        vitesse.y[i] = vitesse.y[i] + acceleration.y[i]*h/2;
    }
    duree = duree + h;
}

std::cout << "nombre d'atomes en interaction avec l'atome =" << n_a << std::endl;
std::cout << "profondeur du puit de potentiel : E_0 =" << E_0 << std::endl;
std::cout << "distance d'annulation du potentiel : d =" << d << std::endl;

std::cout << "Au hasard on affiche la position selon x du " << 10 << "-ième atome : " << position.x[10] << "m" << std::endl;
std::cout << "Au hasard on affiche la position selon y du " << 10 << "-ième atome : " << position.y[10] << "m" << std::endl;

return 0;
}

