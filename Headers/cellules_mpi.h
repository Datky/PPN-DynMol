#pragma once
//#ifndef STRUCT_C
//#define STRUCT_C

#include <iostream>
#include <cstdlib>
#include <vector>
#include "../Headers/types.h"

/** @fichier
 * Structures de données pour représenter les cellules
 **/

/*
struct Cellules {
      std::vector<std::vector<u32>> vec;
};
*/

struct Particule_Cellule_Send {
      f64 X;
      f64 Y;
      f64 Z;
};

struct Particule_Cellule {
      f64 X;
      f64 Y;
      f64 Z;
      int id;
};

struct Cellules {
      std::vector<std::vector<std::vector<std::vector<Particule_Cellule>>>> vec;
};




//#endif //STRUCT_C
