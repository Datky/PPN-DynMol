#include <iostream>
#include <cstdlib>
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


#endif //STRUCT_P