#pragma once
//#ifndef XYZ_H
//#define XYZ_H

/** Un fichier .xyz contient typiquement 
 * la première ligne : le nombre d'atomes,
 * la seconde ligne : un commentaire 
 * et la liste des coordonnées cartésiennes (en ångströms Å) des différents atomes.
 **/

/**    Disposition :
 *
 *   <Nombre d'atomes>
 *   Ligne de commentaire
 *   <Élément 1> <x(1)> <y(1)> <z(1)>
 *   <Élément 2> <x(2)> <y(2)> <z(2)>
 *   …
 *   <Élément n> <x(n)> <y(n)> <z(n)>
 *
 **/


#include "types.h"
#include "../SoA/particule_mpi.h"

//#include "atome.hpp"

/** Lit un fichier au format .xyz , retourne des atomes
 * @param source vers un fichier .xyz
 * @return position des atomes
 **/
void lireXYZ(std::string source, struct Vecteur_3D* pos);
int lire_XYZ_Para(int b_z, int rang, int P, std::string source, struct Vecteur_Para &pos);

/**  Écrit un fichier au format .xyz
 * @param p position des atomes, cible le nom d'un fichier .xyz
 **/
void  ecrireXYZ(struct Vecteur_3D* pos, std::string cible);
void ecrire_XYZ_Para_local(std::string cible, std::vector<u32> &ids, struct Vecteur_Para &pos, int n_local);

//#endif //XYZ_H
