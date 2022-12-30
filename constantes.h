#pragma once

#define N 10000
#define nb_iteration 20
f64 dt=1000;   //en fs (= 10^(-15) s)

//Pour l'argon
#define E_0 9.960719*pow(10,-5)  //en u.Å²/fs² (= 119.8*1.380649*10^(-23) en kg.m²/s² )
#define d 3.405    //en Å (1 Å = 10^(-10) m)
#define m 39.948    //en u (1 u = 1.6605402E-27 kg)

#define k_b 8.314456*pow(10,-7) //en u.Å²/(fs².K) ( constante de Boltzman :	k = 1,380649 × 10−23 kg.m²/(s².K) )
#define temperature_cible 300  // en K

#define b_x 719.14  //en Å
#define b_y 719.14  //en Å
#define b_z 719.14  //en Å
