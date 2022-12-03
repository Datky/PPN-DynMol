/** Un fichier .xyz contient typiquement 
 * la première ligne : le nombre d'atomes,
 * la seconde ligne : un commentaire 
 * et la liste des coordonnées cartésiennes (en ångströms Å) des différents atomes.
 **/

/**    Disposition : n = nombre d'atomes et m = nombre d'iteration
 *
 * <Nombre d'atomes>
 * Ligne de commentaire
 * Symbole_atome11 x-coord11 y-coord11 z-coord11
 * Symbole_atome12 x-coord12 y-coord11 z-coord12
 * …
 * Symbole_atome1n x-coord1n y-coord1n z-coord1n
 * <Nombre d'atomes>
 * Ligne de commentaire
 * Symbole_atome21 x-coord21 y-coord21 z-coord21
 * Symbole_atome22 x-coord22 y-coord21 z-coord22
 * …
 * Symbole_atome2n x-coord2n y-coord2n z-coord2n
 * .
 * .
 * .
 * <Nombre d'atomes>
 * Ligne de commentaire
 * Symbole_atomem1 x-coordm1 y-coordm1 z-coordm1
 * Symbole_atomem2 x-coordm2 y-coordm1 z-coordm2
 * …
 * Symbole_atomemn x-coordmn y-coordmn z-coordmn
 *
 **/

#ifndef XYZ_H
#define XYZ_H

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "types.h"
#include "SoA/particule.h"

/** Lit un fichier au format .xyz , retourne les atomes de la premier iteration
 * @param source vers un fichier .xyz
 * @param source vers un fichier .xyz
 * @return position des atomes
 **/
Vecteur_3D lireXYZ(std::string source);

/**  Écrit un fichier au format .xyz
 * @param p position des atomes
 * @param cible le nom d'un fichier .xyz
 **/
void  ecrireXYZ(Vecteur_3D const &p, std::string cible);

#endif //XYZ_H
