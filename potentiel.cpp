/** @fichier
 * Les fonction d'énergies potentielles de paire
 **/

#include <cmath>
#include "potentiel.h"

// Calcul de la force de Lennard_Jones de manière explicite :
f64 F_Lennard_Jones(f64 const& r){
    f64 tmp_force; 
    if (r<proches) {
        tmp_force = 0; // NEW Abstention des cas où les particules sont trop proches
    } else {
        tmp_force = 48*E_0/r*(pow(d/r,12.0)-1/2*pow(d/r,6.0)); // ok : cf démo
    } 
    return tmp_force; // a priori ok : cf démo
}
