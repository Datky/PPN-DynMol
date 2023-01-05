#pragma once
//#ifndef STRUCT_PA
//#define STRUCT_PA

#include <iostream>
#include <cstdlib>
#include "../types.h"

/** @fichier
 * Structures de données pour représenter les particules
 **/


struct Particule {
      f64 *__restrict pos_X;
      f64 *__restrict pos_Y;
      f64 *__restrict pos_Z;

      f64 *__restrict vit_X;
      f64 *__restrict vit_Y;
      f64 *__restrict vit_Z;

      f64 *__restrict acc_X;
      f64 *__restrict acc_Y;
      f64 *__restrict acc_Z;
};


struct Liste {
      Particule *__restrict liste;
};


//#endif //STRUCT_PA
