// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#ifndef INTERACTION_H
#define INTERACTION_H

#include <cmath>
#include "atome.hpp"

/* Enrgie des atomes
 * @param [int/out] at tablaeu des atome dans la boîte de modélisation
 * @param [out] E tablaeu des énergies d'interaction
 * @param [int] rayon de coupure
 **/
//void energie_atome(Atomes & at, std::vector<f64> & E, f64 const& r_cut);

/* Verlet : Pour trouver la nouvelle position d'un atom
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] dt pas de temps
 **/
void Verlet(Atomes & at, f64 const& dt, f64 const& h, f64 const& r_cut); //f64 & r_max

#endif //INTERACTION_H
