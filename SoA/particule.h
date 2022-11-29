#include <iostream>
#include <cstdlib>
#include "../constantes.h"
#include "../types.h"

/** @fichier
 * Structures de données pour représenter les particules
 **/

#ifndef STRUCT_P
#define STRUCT_P


struct Vecteur_3D {
      f64 *__restrict X;
      f64 *__restrict Y;
      f64 *__restrict Z;
};


struct Particules {
      Vecteur_3D *__restrict pos;
      Vecteur_3D *__restrict vit;
      Vecteur_3D *__restrict acc;
};

/*
struct Vecteur_2D {
      f64 *__restrict X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      f64 *__restrict Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
};


struct Particules {
      Vecteur_2D *__restrict pos = static_cast<Vecteur_2D*>(std::aligned_alloc(sizeof(Vecteur_2D), sizeof(Vecteur_2D)));
      Vecteur_2D *__restrict vit = static_cast<Vecteur_2D*>(std::aligned_alloc(sizeof(Vecteur_2D), sizeof(Vecteur_2D)));
      Vecteur_2D *__restrict acc = static_cast<Vecteur_2D*>(std::aligned_alloc(sizeof(Vecteur_2D), sizeof(Vecteur_2D)));
};

*/

#endif //STRUCT_P