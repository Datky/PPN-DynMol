// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#ifndef INTERACTION_H
#define INTERACTION_H

#include <memory>
#include "types.h"
#include "constantes.h"
#include "SoA/particule.h"

enum class Frontiere {Periodiques, Murs};

/* Verlet : Pour trouver la nouvelle position d'un atom. avec des limites spatial périodiques
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] r_cut distance de coupure
 * @param [int] frontiere_type le type de frontière utiliser
 **/
void Verlet(Particules & at, f64 const& r_cut, Frontiere const& frontiere_type); //f64 & r_max

struct Limites {
      virtual void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z) = 0;
      virtual ~Limites() = default;
};

struct LimitesPeriodiques : public Limites {
      void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z) override {}
};

struct LimitesMurs : public Limites {
      void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z) override {}
};

namespace LimitesFabric {
      std::unique_ptr<Limites> create(Frontiere const& f);
};

#endif //INTERACTION_H
