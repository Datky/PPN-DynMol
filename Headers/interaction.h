#pragma once
//#ifndef INTERACTION_H
//#define INTERACTION_H

// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#include <memory>
#include "potentiel.h"

enum class Class {SoA, AoS};
enum class Optimisation {v0, Liste_voisins, Cellule,  Liste_voisins_et_Cellule};
enum class Frontiere {Periodiques, Murs};

/* Verlet : Pour trouver la nouvelle position d'un atom. avec des limites spatial périodiques
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] r_cut distance de coupure
 * @param [int] frontiere_type le type de frontière utiliser
**/
void Verlet(Particules & at, f64 const& r_cut, Frontiere const& frontiere_type); //f64 & r_max

/*Structure*****************************************************************************************************/
struct Structure {
      virtual ~Structure() = default;
};

struct Structure_SoA : public Structure {};

struct Structure_AoS : public Structure {};

/*Optimisation**************************************************************************************************/
struct Version { 
      virtual ~Version() = default;
};

struct Version0 : public Version {};

struct VersionLV : public Version {};

struct VersionC : public Version {};

struct VersionLVC : public Version {};

/*Frontiere******************************************************************************************************/
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
                  f64 r_x = X+proches;
                  f64 r_y = std::min(Y,b_x-Y);
                  f64 r_z = std::min(Z,b_x-Z);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_global)*r_z/r_global;
                  X = r_cut+1;

             } else if (X > b_x-r_cut) { 
                  f64 r_x = b_x-X-proches;
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_global)*r_z/r_global;
                  X = b_x-r_cut-1;
             } 

             if (Y < r_cut) { 
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = Y+proches;
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_global)*r_z/r_global;
                  Y = r_cut+1;

             } else if (Y > b_y-r_cut) { 
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = b_y-Y-proches;
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_global)*r_z/r_global;
                  Y = b_y-r_cut-1;
             } 
             
             if (Z < r_cut) {
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = Z+proches;
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_global)*r_z/r_global; 
                  Z = r_cut+1;

             } else if (Z > b_z-r_cut) { 
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = b_z-Z-proches;
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                  F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                  F_z += F_Lennard_Jones(r_global)*r_z/r_global;
                  Z = b_z-r_cut-1;
             }
      }
};

/*Fabrication****************************************************************************************************/

namespace StructureFabric {
      std::unique_ptr<Structure> create(Class const& structure_type);
};

namespace VersionFabric {
      std::unique_ptr<Version> create(Optimisation const& version_type);
};

namespace LimitesFabric {
      std::unique_ptr<Limites> create(Frontiere const& frontiere_type);
};

//#endif //INTERACTION_H
