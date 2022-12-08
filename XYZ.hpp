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

#ifndef XYZ_H
#define XYZ_H

#include "types.h"
//#include "atome.hpp"

/** Lit un fichier au format .xyz , retourne des atomes
 * @param source vers un fichier .xyz
 * @return position des atomes
 **/
void lireXYZ(std::string source, struct Vecteur_3D*);

/**  Écrit un fichier au format .xyz
 * @param p position des atomes, cible le nom d'un fichier .xyz
 **/
//void  ecrireXYZ(Position const &p, std::string cible);

#endif //XYZ_H
