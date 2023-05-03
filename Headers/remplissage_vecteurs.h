#pragma once
//#ifndef REMPLISSAGE_VECTEURS_H
//#define REMPLISSAGE_VECTEURS_H

/* ...
 * ...
 **/

void remplissage_vecteurs(struct Vecteur_3D*, struct Vecteur_3D*, struct Vecteur_3D*);
void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit);
void accelerations_initiales_nulles(struct Vecteur_3D* acc);


int remplissage_vecteurs_para(int b_z, int rang, int P, std::vector<u64> &ids, struct Vecteur_Para &pos, struct Vecteur_Para &vit, struct Vecteur_Para &acc);
void remplissage_ids(int somme_prev, int n_local, std::vector<u64> &ids);
int generation_positions(int b_z, int rang, int P, struct Vecteur_Para &pos);
void generation_gaussienne_des_vitesses_para(int n_local, struct Vecteur_Para &vit);
void accelerations_initiales_nulles_para(int n_local, struct Vecteur_Para &acc);

//#endif // REMPLISSAGE_VECTEURS_H
