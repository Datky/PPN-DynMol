#include <vector>
#include <random>

#include "type.h"
/** @fichier
 * Structures de données pour représenter les atomes
 **/

#ifndef STRUCT_H
#define STRUCT_H

struct Position{
      std::vector<f64> x;  //Position sur l'axe des x
      std::vector<f64> y;  //Position sur l'axe des y
      std::vector<f64> z;  //Position sur l'axe des x
      //Peut être écrire les operateur?//
};

struct Atome {
      Position p;          //Position
      std::vector<f64> v;  //La vitesse
      std::vector<f64> a;  //L'acceleration
      std::vector<f64> E;  //Energie potentielle
};


/*struct Cellule{
      bool atome;   //Si contin un atome true, sinon false
};*/

#endif //STRUCT_H

#ifndef ATOM_FUNC_H
#define ATOM_FUNC_H

struct Vec3d
{
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
};

struct Limites_du_domaine {
  Vec3d lmin;
  Vec3d lmax;
}; // AABB style (axis-aligned bounding boxes)

void generation_aleatoire_uniforme_des_atomes(Atome *A, u64 N, Limites_du_domaine *L){
  
  // D'après :
  // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

  // std::random_device rd;  // Will be used to obtain a seed for the random number engine
  // std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  // std::uniform_real_distribution<> dis(1.0, 2.0);
  // for (int n = 0; n < 10; ++n) {
  //   // Use dis to transform the random unsigned int generated by gen into a 
  //   // double in [1, 2). Each call to dis(gen) generates a new random double
  //   std::cout << dis(gen) << ' ';
  // }
  // std::cout << '\n';

  std::random_device rd;  
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis_x(L->lmin.x, L->lmax.x);
  std::uniform_real_distribution<> dis_y(L->lmin.y, L->lmax.y);
  std::uniform_real_distribution<> dis_z(L->lmin.z, L->lmax.z);

  A->p.x.reserve(N);
  A->p.y.reserve(N);
  A->p.z.reserve(N);
  
  for(u64 i = 0; i < N; i++){
    A->p.x.push_back(dis_x(gen));
    A->p.y.push_back(dis_y(gen));
    A->p.z.push_back(dis_z(gen));
  }
};

#endif //ATOM_FUNC_H
