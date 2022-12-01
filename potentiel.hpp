#ifndef POTENTIEL_H
#define POTENTIEL_H

#include "atome.hpp"

/* Le potentiel de Lennard-Jones : 
 * énergie potentielle qui décrit l'interaction entre 2 atomes au sein d'un gaz monoatomique de type gaz rare. 
 * @param r distanse entre 2 atomes
 * @return énergie potentiel entre 2 atomes
 **/
f64 Lennard_Jones(f64 const& r);

#endif //POTENTIEL_H
