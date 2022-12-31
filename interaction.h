// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#ifndef INTERACTION_H
#define INTERACTION_H

#include <memory>
#include "types.h"
#include "constantes.h"
#include "SoA/particule.h"
#include "potentiel.h"

enum class Frontiere {Periodiques, Murs};

/* Verlet : Pour trouver la nouvelle position d'un atom. avec des limites spatial périodiques
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] r_cut distance de coupure
 * @param [int] frontiere_type le type de frontière utiliser
 **/
void Verlet(Particules & at, f64 const& r_cut, Frontiere const& frontiere_type); //f64 & r_max

struct Limites {
      virtual void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z, f64 const& r_cut) = 0;
      virtual ~Limites() = default;
};

struct LimitesPeriodiques : public Limites {
      void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z, f64 const& r_cut) override {
             // Limites spatiales périodiques avec origine en (0,0,0)
             if (X < 0) {X = b_x;}
             else if (X > b_x) {X = 0;} 

             if (Y < 0) {Y = b_y;}
             else if (Y > b_y) {Y = 0;} 
             
             if (Z < 0) {Z = b_z;}
             else if (Z > b_z) {Z = 0;} 
      }
};

struct LimitesMurs : public Limites {
      void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z, f64 const& r_cut) override {
             // Murs au frontière de la boîte
             if (X < r_cut) { 
                  f64 r_x = abs(X);
                  f64 r_y = std::max(abs(Y),abs(b_x-Y));
                  f64 r_z = std::max(abs(Z),abs(b_x-Z));
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_z)*r_z/r_global;

             } else if (X > b_x-r_cut) { 
                  f64 r_x = abs(b_x-X);
                  f64 r_y = std::max(abs(Y),abs(b_y-Y));
                  f64 r_z = std::max(abs(Z),abs(b_z-Z));
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_z)*r_z/r_global;
             } 

             if (Y < r_cut) { 
                  f64 r_x = std::max(abs(X),abs(b_x-X));
                  f64 r_y = abs(Y);
                  f64 r_z = std::max(abs(Z),abs(b_z-Z));
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_z)*r_z/r_global;

             } else if (Y > b_y-r_cut) { 
                  f64 r_x = std::max(abs(X),abs(b_x-X));
                  f64 r_y = abs(b_y-Y);
                  f64 r_z = std::max(abs(Z),abs(b_z-Z));
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_z)*r_z/r_global;
             } 
             
             if (Z < r_cut) {
                  f64 r_x = std::max(abs(X),abs(b_x-X));
                  f64 r_y = std::max(abs(Y),abs(b_y-Y));
                  f64 r_z = abs(Z);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_z)*r_z/r_global; 

             } else if (Z > b_z-r_cut) { 
                  f64 r_x = std::max(abs(X),abs(b_x-X));
                  f64 r_y = std::max(abs(Y),abs(b_y-Y));
                  f64 r_z = abs(b_z-Z);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_z)*r_z/r_global;
             }
      }
};

namespace LimitesFabric {
      std::unique_ptr<Limites> create(Frontiere const& f);
};

#endif //INTERACTION_H
