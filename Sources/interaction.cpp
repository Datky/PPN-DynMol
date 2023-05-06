/** @fichier
 * Algorithmes
 **/

#include <cmath>
#include <vector>
#include "../Headers/interaction.h"
//#include <vector>

//std::vector<int> NN, NL;
extern u32 N;
extern u32 nb_iteration;
extern u32 dt;
extern u32 b_x; // NEW
extern u32 b_y; // NEW
extern u32 b_z; // NEW
extern u32  MN;
//const int MN = 10000;
/*
void  Voisin(Particules & at, f64 const& r_cut) {

        double r_cut2 = r_cut * r_cut;

        std::fill(NN.begin(), NN.end(), 0);

                for (int i = 0; i < N; ++i) {
                        for (int j = i + 1; j < N; ++j) {

                                // Calculate the distance between the two particles          
                                double r_x = abs(at.pos->X[j] - at.pos->X[i]);
                                double r_y = abs(at.pos->Y[j] - at.pos->Y[i]);
                                double r_z = abs(at.pos->Z[j] - at.pos->Z[i]);
                                rayonverlet(b_x, b_y, b_z, r_x, r_y, r_z);
                                const double r_global = sqrt(pow(r_x, 2.0) + pow(r_y, 2.0) + pow(r_z, 2.0));
                  if (r_global < r_cut && r_global != 0) {

                                        NL[i * MN + NN[i]++] = j;
                                        if (NN[i] > MN) {
                                                std::cout << "Erreur : nombre de voisins pour l'atome " << i << " exceeds " << MN << std::endl;
                                                exit(1);

                                        }


                                        }

                        }
                }
}
*/           


void majPositionsetCellules(std::vector<std::vector<std::vector<std::vector<u32>>>> &vec, Particules &at, f64 const& r_cut_carre, Frontiere const& frontiere_type, f64 max_displacement) {

      f64 F_x, F_y, F_z ;
      F_x = F_y = F_z = 0; // ????

      
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);

          // Taille des cellules
          f64 tc_x = b_x / c_x;
          f64 tc_y = b_y / c_y;
          f64 tc_z = b_z / c_z;

      for (u32 i = 0; i < N; ++i) {


            int old_z = at.pos->Z[i] / tc_x;
            int old_y = at.pos->Y[i] / tc_y;
            int old_x = at.pos->X[i] / tc_z;

	    f64 old_x_pos = at.pos->X[i];
	    f64 old_y_pos = at.pos->Y[i];
	    f64 old_z_pos = at.pos->Z[i];


            // Calcul des positions
            at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
            at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
            at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

	    f64 displacement = sqrt(pow(at.pos->X[i] - old_x_pos, 2) + pow(at.pos->Y[i] - old_y_pos, 2) + pow(at.pos->Z[i] - old_z_pos, 2));
	    
	    max_displacement = std::max(max_displacement, displacement);

            // Mise en place d'une frontière
            unique_limites->creeLimites(at.pos->X[i], at.pos->Y[i], at.pos->Z[i], F_x, F_y, F_z, r_cut_carre);


            // Calcul de la nouvelle cellule de la particule
            int new_z = at.pos->Z[i] / tc_z;
            int new_y = at.pos->Y[i] / tc_y;
            int new_x = at.pos->X[i] / tc_x;



            if (old_x != new_x || old_y != new_y || old_z != new_z) { // La particule change de cellule
                  // Cherche la position de l'élément pour le supprimer
                  for (u32 id = 0; id < vec[old_z+1][old_y+1][old_x+1].size(); ++id) {
                        if (vec[old_z+1][old_y+1][old_x+1][id] == i) {
                              vec[old_z+1][old_y+1][old_x+1].erase(vec[old_z+1][old_y+1][old_x+1].begin()+id, vec[old_z+1][old_y+1][old_x+1].begin()+id+1);
                              break;
                        }
                  }
                  // Réinsertion dans la bonne cellule
                  vec[new_z+1][new_y+1][new_x+1].push_back(i);
            }
      }
}


void VerletCellules(std::vector<std::vector<std::vector<std::vector<u32>>>> &vec, Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type, std::vector<u32>& NN, std::vector<u32>& NL) {

      f64 F_x, F_y, F_z ;
      // Mise en place d'une frontière : création du pointeur
      //auto unique_limites = LimitesFabric::create(frontiere_type);
/*
      f64 r_cut=sqrt(r_cut_carre);
      f64 update = 0.5*r_cut;

      
 */
      auto unique_limites = LimitesFabric::create(frontiere_type);



      // Boucle dans les cellules en évitant les ghost cell
      for (int i = 1; i < c_z+1; ++i) {
            for (int j = 1; j < c_y+1; ++j) {
                  for (int k = 1; k < c_x+1; ++k) {


                        // Verlet
                        for (u32 ind = 0; ind < vec[i][j][k].size(); ++ind) {

				u32 particule = vec[i][j][k][ind];
                              // Mise à zéro de la force
                              F_x = F_y = F_z = 0; 

//                              int particule = vec[i][j][k][ind];



                              // 1er calcul des vitesses : v_i(t+dt/2)
                              at.vit->X[particule] += 0.5*at.acc->X[particule]*dt;
                              at.vit->Y[particule] += 0.5*at.acc->Y[particule]*dt;
                              at.vit->Z[particule] += 0.5*at.acc->Z[particule]*dt;



                              // Calcul de la force : F_i(t+dt) et a_i(t+dt)
//                            u32 particule = vec[i][j][k][ind];
			    for (u32 k=0; k<NN[particule];++k){
				    u32 j= NL[particule * MN + k];
			     /* for (int ii = i-1; ii <= i+1; ++ii) {
                                    for (int jj = j-1; jj <= j+1; ++jj) {
                                          for (int kk = k-1; kk <= k+1; ++kk) {
                                                for (u32 vois = 0; vois < vec[ii][jj][kk].size(); ++vois) {
						*/
                                                      //f64 r_x = at.pos->X[particule] - at.pos->X[vec[ii][jj][kk][vois]];
                                                      //f64 r_y = at.pos->Y[particule] - at.pos->Y[vec[ii][jj][kk][vois]];
                                                      //f64 r_z = at.pos->Z[particule] - at.pos->Z[vec[ii][jj][kk][vois]];
                                                      f64 r_x = unique_limites->calculDistance(at.pos->X[particule], at.pos->X[j], b_x, r_cut_carre);
                                                      f64 r_y = unique_limites->calculDistance(at.pos->Y[particule], at.pos->Y[j], b_y, r_cut_carre);
                                                      f64 r_z = unique_limites->calculDistance(at.pos->Z[particule], at.pos->Z[j], b_z, r_cut_carre);
                                                      f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                                                      if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                                                      F_x += F_Lennard_Jones(r_global_carre)*r_x;
                                                      F_y += F_Lennard_Jones(r_global_carre)*r_y;
                                                      F_z += F_Lennard_Jones(r_global_carre)*r_z;

                                                      }
                                                }
                                         // }
                                   // }
                             // }

                              // Calcul des accélérations : a_i(t+dt)
                              at.acc->X[particule] = F_x/m;
                              at.acc->Y[particule] = F_y/m;
                              at.acc->Z[particule] = F_z/m;

                              // 2ième calcul des vitesses : v_i(t+dt)
                              at.vit->X[particule] += 0.5*at.acc->X[particule]*dt;
                              at.vit->Y[particule] += 0.5*at.acc->Y[particule]*dt;
                              at.vit->Z[particule] += 0.5*at.acc->Z[particule]*dt;
                        }
                  }
            }
      }
}

// Algorithme de Verlet
void Verlet(Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type){

      f64 F_x, F_y, F_z ;
      
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);

      for(u32 i=0; i<N; ++i){

            // Mise à zéro de la force
            F_x = F_y = F_z = 0;  

             // Calcul des positions : p_i(t+dt)
             at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
             at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
             at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

             // Mise en place d'une frontière
             unique_limites->creeLimites(at.pos->X[i], at.pos->Y[i], at.pos->Z[i], F_x, F_y, F_z, r_cut_carre);

             // 1er calcul des vitesses : v_i(t+dt/2)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

             // Calcul de la force : F_i(t+dt) et a_i(t+dt)
             for(u32 j=0; j<N; ++j){ 

/*                   
                   //Ne parcourir que la liste de voisin//
                   for (int jj = 0; jj < NN[i]; ++jj){
                         const int j = NL[i * MN + jj];// Calcul de la distance entre les atomes : r_i(t+dt)

                         f64 r_x = at.pos->X[i] - at.pos->X[j];
                         f64 r_y = at.pos->Y[i] - at.pos->Y[j];
                         f64 r_z = at.pos->Z[i] - at.pos->Z[j];
*/
//                         rayonverlet(b_x, b_y, b_z, r_x, r_y, r_z);
                         // f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0)); !AVANT!


                         f64 r_x = unique_limites->calculDistance(at.pos->X[i], at.pos->X[j], b_x, r_cut_carre);
                         f64 r_y = unique_limites->calculDistance(at.pos->Y[i], at.pos->Y[j], b_y, r_cut_carre);
                         f64 r_z = unique_limites->calculDistance(at.pos->Z[i], at.pos->Z[j], b_z, r_cut_carre);

                         
                         f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);
                         // Calcul de la force si la distance inter-atomique globale est inférieure au rayon de coupure :

                         // if (r_global<r_cut && r_global!=0) { !AVANT!                 
                         if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                               F_x += F_Lennard_Jones(r_global_carre)*r_x;
                               F_y += F_Lennard_Jones(r_global_carre)*r_y;
                               F_z += F_Lennard_Jones(r_global_carre)*r_z;

                                // Peut être pas nécessaire
                        // Stocker dans la liste de voisin
               
                        // at.liste[i].append(j)
                     //}    
                   }
             }

             // Calcul des accélérations : a_i(t+dt)
             at.acc->X[i] = F_x/m;
             at.acc->Y[i] = F_y/m;
             at.acc->Z[i] = F_z/m;

             // 2ième calcul des vitesses : v_i(t+dt)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;
      }   
}

/*Fabrication****************************************************************************************************/
std::unique_ptr<Limites> LimitesFabric::create(Frontiere const& frontiere_type) {
      std::unique_ptr<Limites> unique_limites;

      switch (frontiere_type) {
            case Frontiere::Periodiques:
                  unique_limites = std::make_unique<LimitesPeriodiques>();
                  break;

            case Frontiere::Murs:
                  unique_limites = std::make_unique<LimitesMurs>();
                  break;
      }
      return unique_limites;
}

