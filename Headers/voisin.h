#pragma once
//#ifndef POTENTIEL_H
//#define POTENTIEL_H

#include "types.h"
#include "constantes.h"
#include "../SoA/particule.h"


//using namespace std;

using namespace std;
extern u32 N;


extern u32 MN;
extern std::vector<u32> NN;
extern std::vector<u32> NL;

f64 rayonverlet1(double const& boxLength, double const& halfLength, double xij);
void rayonverlet(double bx, double by, double bz, double& x12, double& y12, double& z12);
void Voisin(Particules & p, f64 r_cut, f64 b_x, f64 b_y, f64 b_z, std::vector<u32>& NN, std::vector<u32>& NL);

/*

f64 rayonverlet1(f64 const& boxLength, f64 const& halfLength, f64 xij);

void rayonverlet(f64 bx, f64 by, f64 bz, f64 x12, f64 y12, f64 z12);

void Voisin(Particules & p, f64 r_cut);
*/
