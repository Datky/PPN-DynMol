#pragma once

#include "types.h"
#define proches 1 // Si la distance est < 1 Å, les particules sont considérées trop proches et le potentiel négligé
/*
#define N 10000
#define nb_iteration 5 //NEW
#define dt 1000   //en fs (= 10^(-15) s)
*/

//Pour l'argon
#define E_0 9.960719*pow(10,-5)  //en u.Å²/fs² (= 119.8*1.380649*10^(-23) en kg.m²/s² )
#define d 3.405    //en Å (1 Å = 10^(-10) m)
#define d_carre 11.594    //3,045² //!NOUVEAU!
#define m 39.948    //en u (1 u = 1.6605402E-27 kg)

#define k_b 8.314456*pow(10,-7) //en u.Å²/(fs².K) ( constante de Boltzman :	k = 1,380649 × 10−23 kg.m²/(s².K) )
#define temperature_cible 300  // en K

#define b_x 719.14  //en Å
#define b_y 719.14  //en Å
#define b_z 719.14  //en Å

// Nombre de cellules dans la boîte
#define c_x 5
#define c_y 5
#define c_z 5

extern const u32 N;
extern const u32 nb_iteration;
extern const u32 dt;

struct parametres_N {
    u32 N;
    u32 parametres();
};

struct parametres_nb_iteration {
    u32 nb_iteration;
    u32 parametres();
};

struct parametres_dt {
    u32 dt;
    u32 parametres();
};

