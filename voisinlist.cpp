#include "voisinlist.h" 


// Type aliases to make the code more readable
using Vecteur3D = Vecteur_3D;
using Particules = struct Particules;

// Struct to represent a pair of particle indices and their distance
struct ParticlePair {
     int i, j;
  double distance;
};



