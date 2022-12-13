// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#ifndef INTERACTION_H
#define INTERACTION_H

/* Verlet : Pour trouver la nouvelle position d'un atom. avec des limites spatial périodiques
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] dt pas de temps
 * @param [int] r_cut distance de coupure
 **/
void Verlet(Particules & at, f64 const& dt, f64 const& r_cut); //f64 & r_max

#endif //INTERACTION_H
