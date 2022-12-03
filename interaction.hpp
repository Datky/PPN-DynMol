// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#ifndef INTERACTION_H
#define INTERACTION_H

#include "SoA/particule.h"

/* Verlet : Pour trouver la nouvelle position d'un atom
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] dt pas de temps
 * @param [int] r_cut distance de coupure
 * @param [int] m la masse
 **/
void Verlet(Particules & at, f64 const& dt, f64 const& r_cut, f64 const& m); //f64 & r_max

#endif //INTERACTION_H
