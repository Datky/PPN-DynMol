/** @fichier
 * Les fonction d'énergies potentielles de paire
 **/

#include <cmath>
#include "potentiel.h"

// Calcul du potentiel de Lennard_Jones :
f64 Lennard_Jones(f64 const& r){
    return 4*E_0*(pow(d/r,12.0)-pow(d/r,6.0));
}
// ? Optimisation 4*E_0*(pow(d,12.0)/pow(r²,6.0)-pow(d,6.0)/pow(r²,3.0));

// Calcul de la force de Lennard_Jones de manière explicite :
f64 F_Lennard_Jones(f64 const& r){
    f64 tmp_force; 
    if (r<d) {
        tmp_force = 0; // Abstention des cas où la force est répulsive.
    } else {
        tmp_force = 48*E_0/r*(pow(d/r,12.0)-1/2*pow(d/r,6.0)); // a priori ok : cf démo
    } 
    return tmp_force; // a priori ok : cf démo
}
