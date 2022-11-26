// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

/* Enrgie des atomes
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [out] E tablaeu des énergies d'interaction
 * @param [int] rayon de coupure
 **/
void energie_atome(les_atomes & at, std::vector<f64> & E, f64 const& r_cut);

/* Verlet : Pour trouver la nouvelle position d'un atom
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] dt pas de temps
 **/
void Verlet(Tous les atomes & at, f64 & r_max, f64 const& dt);
