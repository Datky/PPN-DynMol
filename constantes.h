#pragma once

#define N 10000
#define nb_iteration 10
f64 dt=pow(10,-15);   //en s (= 1 fm)

//Pour l'argon
#define E_0 119.8*1.380649*pow(10,-23)  //en kg.m²/s²
#define d 3.405*pow(10,-9)    //en m
#define m 6.6335*pow(10,-26)    //en kg (= 39,948 ± 0,001 u)

#define k_b 1.380649*pow(10,-23) // constante de Boltzman :	k = 1,380649 × 10−23 J.K−1
#define temperature_cible 10    // degrés celsius

#define b_x 719.14*pow(10,-10)  //en m (= 719.14 Å)
#define b_y 719.14*pow(10,-10)  //en m
#define b_z 719.14*pow(10,-10)  //en m
