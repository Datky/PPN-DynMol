/** @fichier
 * Les fonction d'énergies potentielles de paire
 **/

#include <cmath>
#include "../Headers/potentiel.h"

// Calcul de la force de Lennard_Jones de manière explicite :
f64 F_Lennard_Jones(f64 const& r){
    f64 tmp_force; 
        // tmp_force = 48*E_0/r*(pow(d/r,12.0)-(pow(d/r,6.0)/2)); // ok : cf démo // !AVANT!
        tmp_force = 48*E_0/r*(pow(d_carre/r,6.0)-(pow(d_carre/r,3.0)/2)); // ok : cf démo // !NOUVEAU! économie de NxNx(6+3) multiplications
    return tmp_force; // a priori ok : cf démo
}
