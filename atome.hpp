#include <vector>

/** @fichier
 * Structures de données pour représenter les atomes
 **/

#ifndef STRUCT_H
#define STRUCT_H

#include "types.h"

struct Vector_3D{
      std::vector<f64> x;  //Positions sur l'axe des x
      std::vector<f64> y;  //Positions sur l'axe des y
      std::vector<f64> z;  //Positions sur l'axe des x
      
      /*void operator= (Vector_3D & v);
      void Vector_3D::operator+= (Vector_3D & v)
      Vector_3D& operator+ (Vector_3D const& v);
      Vector_3D& operator- (Vector_3D const& v);*/
};

struct Atomes {
      Vector_3D p;  //ositions
      Vector_3D v;  //La vitesses
      Vector_3D a;  //L'accelerations
      std::vector<int> m; //La masse
      //std::vector<std::vector<f64>> E;  //Energies potentielles
};


/*struct Cellule{
      bool atome;   //Si contin un atome true, sinon false
};*/

#endif //STRUCT_H
