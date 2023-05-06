#pragma once
//#ifndef POTENTIEL_H
//#define POTENTIEL_H

#include "types.h"
#include "constantes.h"
#include "../SoA/particule.h"

/* Le potentiel de Lennard-Jones : 
 * énergie potentielle qui décrit l'interaction entre 2 atomes au sein d'un gaz monoatomique de type gaz rare.
 **/

/* La force de Lennard-Jones : 
 * force qui est calculée par le négatif du gradient du potentiel de Lennard-Jones.  
 * @param r distanse entre 2 atomes
 * @return force entre 2 atomes
 **/
f64 F_Lennard_Jones(f64 const& r);

//#endif //POTENTIEL_H
