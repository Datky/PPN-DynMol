#include <vector>

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
