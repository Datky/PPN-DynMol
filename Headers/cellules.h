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
struct Cellules {
      std::vector<std::vector<std::vector<std::vector<u32>>>> vec;
};




//#endif //STRUCT_C
