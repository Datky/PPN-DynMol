/** @fichier
 * Les fonction d'éneregies potentiels de paire
 **/

#include "potentiel.h"

// 
f64 Lennard_Jones(f64 const& r){
      return 4*E_0*(pow(d/r,12.0)-pow(d/r,6.0));
}
//Optimisation 4*E_0*(pow(d,12.0)/pow(r²,6.0)-pow(d,6.0)/pow(r²,3.0));

f64 F_Lennard_Jones(f64 const& r){
      //F = - dU/dr
//      return 24*E_0*pow(d,6.0)*(( 2*pow(d,6.0) - pow(r,6.0) ) / pow(r,13.0));
// BONNE EXPRESSION :
        return 48*E_0/d*(pow(d/r,13.0)-1/2*pow(d/r,7.0));
}

//12*pow(d,12.0)/pow(r,13.0) - 6*pow(d,6.0)/pow(r,7.0) = 6*pow(d,6.0)*( 2*pow(d,6.0) - pow(r,6.0) ) / pow(r,13.0)
