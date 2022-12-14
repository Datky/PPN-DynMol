/** @fichier
 * Algorithmes
 **/

#include <cmath>
#include "../Headers/interaction.h"
//#include "rayonverlet.cpp"
//#include <vector>

//std::vector<int> NN, NL;
const int MN = 10000;
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

// Algorithme de Verlet
void Verlet(Particules & at, f64 const& r_cut, Frontiere const& frontiere_type){

      f64 F_x, F_y, F_z ;
      
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);

      for(u32 i=0; i<N; ++i){

            // Mise à zéro de la force
            F_x = F_y = F_z = 0;  
            
  

            /*
            // Déplacement dans ce pas de temps.
            f64 deplac_x = at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
            f64 deplac_y = at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
            f64 deplac_z = at.vit-Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);
            f64 deplac_global = sqrt(pow(deplac_x,2.0) + pow(deplac_y,2.0) + pow(deplac_z,2.0));

            // Stocker plus grand déplacement dans ce pas de temps. f64 r_max  
            if( r_max < deplac_global ) r_max = deplac_global; */

             // Calcul des positions : p_i(t+dt)
             at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
             at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
             at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

             // Mise en place d'une frontière
             unique_limites->creeLimites(at.pos->X[i], at.pos->Y[i], at.pos->Z[i], F_x, F_y, F_z, r_cut);

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
*/
                         f64 r_x = at.pos->X[i] - at.pos->X[j];
                         f64 r_y = at.pos->Y[i] - at.pos->Y[j];
                         f64 r_z = at.pos->Z[i] - at.pos->Z[j];
//                         rayonverlet(b_x, b_y, b_z, r_x, r_y, r_z);
                         f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));
                         // Calcul de la force si la distance inter-atomique globale est inférieure au rayon de coupure :
                         
                         if (r_global<r_cut && r_global!=0) {
                               F_x += F_Lennard_Jones(r_global)*r_x/r_global;
                               F_y += F_Lennard_Jones(r_global)*r_y/r_global;
                               F_z += F_Lennard_Jones(r_global)*r_z/r_global;
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
std::unique_ptr<Structure> StructureFabric::create(Class const& structure_type) {
      std::unique_ptr<Structure> unique_structure;

      switch (structure_type) {
            case Class::SoA:
                  unique_structure = std::make_unique<Structure_SoA>();
                  break;

            case Class::AoS:
                  unique_structure = std::make_unique<Structure_AoS>();
                  break;
      }
      return unique_structure;
}

std::unique_ptr<Version> VersionFabric::create(Optimisation const& version_type) {
      std::unique_ptr<Version> unique_version;

      switch (version_type) {
            case Optimisation::v0:
                  unique_version = std::make_unique<Version0>();
                  break;

            case Optimisation::Liste_voisins:
                  unique_version = std::make_unique<VersionLV>();
                  break;

            case Optimisation::Cellule:
                  unique_version = std::make_unique<VersionC>();
                  break;

            case Optimisation::Liste_voisins_et_Cellule:
                  unique_version = std::make_unique<VersionLVC>();
                  break;
      }
      return unique_version;
}

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

