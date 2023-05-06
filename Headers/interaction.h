#pragma once
//#ifndef INTERACTION_H
//#define INTERACTION_H

// Second principe de la dynamique F = m.a avec un potentiel empirique

//Pseudo code //

#include <memory>
#include <vector>
#include <mpi.h>
#include "potentiel.h"
#include "cellules.h"

extern u32 b_x; // NEW
extern u32 b_y; // NEW
extern u32 b_z; // NEW

enum class Class {SoA, AoS};
enum class Optimisation {v0, Liste_voisins, Cellule,  Liste_voisins_et_Cellule};
enum class Frontiere {Periodiques, Murs};

/* Verlet : Pour trouver la nouvelle position d'un atom. avec des limites spatial périodiques
 * @param [int/out] at tablaeu des les atome dans la boîte de modélisation
 * @param [int] r_cut distance de coupure
 * @param [int] frontiere_type le type de frontière utiliser
**/
void Verlet(Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type); //f64 & r_max
void VerletCellules(std::vector<std::vector<std::vector<std::vector<u32>>>> &vec, Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type); //f64 & r_max
void VerletCellulesPara(int rang, int P, int n_local, int cellules_locales, std::vector<std::vector<std::vector<std::vector<Particule_Cellule>>>> &vec, 
      Particules_Para &part, f64 const& r_cut_carre, Frontiere const& frontiere_type, MPI_Datatype* types, 
      u32** comms);

int trouver_ind(int valeur, std::vector<u32> &vec);

void majPositionsetCellules(std::vector<std::vector<std::vector<std::vector<u32>>>> &vec, Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type);

/*Frontiere******************************************************************************************************/
struct Limites {
      virtual void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z, f64 const& r_cut_carre) = 0;
/*!NOUVEAU!*/
      virtual f64 calculDistance(f64 const& i, f64 const& j, f64 const& b, f64 const& r_cut_carre) = 0;
      virtual ~Limites() = default;
};

struct LimitesPeriodiques : public Limites {
      void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z, f64 const& r_cut_carre) override {
             // Limites spatiales périodiques avec origine en (0,0,0)
             while (X < 0) {X = b_x+X;}
             while (X > b_x) {X = X-b_x;} 

             while (Y < 0) {Y = b_y+Y;}
             while (Y > b_y) {Y = Y-b_y;} 
             
             while (Z < 0) {Z = b_z+Z;}
             while (Z > b_z) {Z = Z-b_z;} 
      }

/*!NOUVEAU!*/
      f64 calculDistance(f64 const& i, f64 const& j, f64 const& b, f64 const& r_cut_carre) override {
            f64 r1 = i-j;
            f64 r1_carre = r1*r1;

            if ( r1_carre > r_cut_carre) {
                  f64 r2;
                  if(r1>0) { r2 = r1-b; }
                  else { r2 = r1+b; }

                  return r2;
            }
            return r1;
      }
};

struct LimitesMurs : public Limites {
      void creeLimites(f64 & X, f64 & Y, f64 & Z, f64 & F_x, f64 & F_y, f64 & F_z, f64 const& r_cut) override {
             // Murs au frontière de la boîte
             if (X < r_cut) { 
                  f64 r_x = X+1;
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                  F_x += F_Lennard_Jones(r_global_carre)*r_x;
                  F_y += F_Lennard_Jones(r_global_carre)*r_y;
                  F_z += F_Lennard_Jones(r_global_carre)*r_z;
                  X = r_cut+1;

             } else if (X > b_x-r_cut) { 
                  f64 r_x = b_x-X-1;
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                  F_x += F_Lennard_Jones(r_global_carre)*r_x;
                  F_y += F_Lennard_Jones(r_global_carre)*r_y;
                  F_z += F_Lennard_Jones(r_global_carre)*r_z;
                  X = b_x-r_cut-1;
             } 

             if (Y < r_cut) { 
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = Y+1;
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                  F_x += F_Lennard_Jones(r_global_carre)*r_x;
                  F_y += F_Lennard_Jones(r_global_carre)*r_y;
                  F_z += F_Lennard_Jones(r_global_carre)*r_z;
                  Y = r_cut+1;

             } else if (Y > b_y-r_cut) { 
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = b_y-Y-1;
                  f64 r_z = std::min(Z,b_z-Z);
                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                  F_x += F_Lennard_Jones(r_global_carre)*r_x;
                  F_y += F_Lennard_Jones(r_global_carre)*r_y;
                  F_z += F_Lennard_Jones(r_global_carre)*r_z;
                  Y = b_y-r_cut-1;
             } 
             
             if (Z < r_cut) {
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = Z+1;
                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                  F_x += F_Lennard_Jones(r_global_carre)*r_x;
                  F_y += F_Lennard_Jones(r_global_carre)*r_y;
                  F_z += F_Lennard_Jones(r_global_carre)*r_z;
                  Z = r_cut+1;

             } else if (Z > b_z-r_cut) { 
                  f64 r_x = std::min(X,b_x-X);
                  f64 r_y = std::min(Y,b_y-Y);
                  f64 r_z = b_z-Z-1;
                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                  F_x += F_Lennard_Jones(r_global_carre)*r_x; 
                  F_y += F_Lennard_Jones(r_global_carre)*r_y;
                  F_z += F_Lennard_Jones(r_global_carre)*r_z;
                  Z = b_z-r_cut-1;
             }
      }

/*!NOUVEAU!*/
      f64 calculDistance(f64 const& i, f64 const& j, f64 const& b, f64 const& r_cut_carre) override {
            return i - j;
      }
};

/*Fabrication****************************************************************************************************/

namespace LimitesFabric {
      std::unique_ptr<Limites> create(Frontiere const& frontiere_type);
};

//#endif //INTERACTION_H
