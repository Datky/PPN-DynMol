#ifndef REMPLISSAGE_VECTEURS_H
#define REMPLISSAGE_VECTEURS_H

/* ...
 * ...
 **/

void remplissage_vecteurs(struct Vecteur_3D*, struct Vecteur_3D*, struct Vecteur_3D*);
void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit);
void accelerations_initiales_nulles(struct Vecteur_3D* acc);

#endif // REMPLISSAGE_VECTEURS_H
