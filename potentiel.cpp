/** @fichier
 * Les fonction d'éneregies potentiels de paire
 **/

#include "potentiel.h"

// 
f64 Lennard_Jones(f64 const& r){
      return 4*E_0*(pow(d/r,12.0)-pow(d/r,6.0));
}
//Optimisation 4*E_0*(pow(d,12.0)/pow(r²,6.0)-pow(d,6.0)/pow(r²,3.0));
