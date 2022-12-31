/** @fichier
 * Algorithmes
 **/

#include <cmath>
#include "interaction.h"
//#include "voisinlist.cpp"

// Algorithme de Verlet
void Verlet(Particules & at, f64 const& r_cut, Frontiere const& frontiere_type){

      f64 F_x, F_y, F_z ;

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
             auto unique_limites = LimitesFabric::create(frontiere_type);
             unique_limites->creeLimites(at.pos->X[i], at.pos->Y[i], at.pos->Z[i], F_x, F_y, F_z, r_cut);

             // 1er calcul des vitesses : v_i(t+dt/2)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

             // Calcul de la force : F_i(t+dt) et a_i(t+dt)
             for(u32 j=0; j<N; ++j){        //Ne parcourir que la liste de voisin//

                  // Calcul de la distance entre les atomes : r_i(t+dt)
                  f64 r_x = abs(at.pos->X[j] - at.pos->X[i]);
                  f64 r_y = abs(at.pos->Y[j] - at.pos->Y[i]);
                  f64 r_z = abs(at.pos->Z[j] - at.pos->Z[i]);
                  f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  // Calcul de la force si la distance inter-atomique globale est inférieure au rayon de coupure :
                  if (r_global<r_cut && r_global!=0) {

                        F_x += F_Lennard_Jones(r_x)*r_x/r_global;
                        F_y += F_Lennard_Jones(r_y)*r_y/r_global;
                        F_z += F_Lennard_Jones(r_z)*r_z/r_global;

                        // Peut être pas nécessaire
                        // Stocker dans la liste de voisin
                        // nearestVoisin();
                        // at.liste[i].append(j)
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

std::unique_ptr<Limites> LimitesFabric::create(Frontiere const& frontiere_type) {
      std::unique_ptr<Limites> unique_limites;

      switch (frontiere_type) {
            case Frontiere::Periodiques:
                  unique_limites = std::make_unique<LimitesPeriodiques>();

            case Frontiere::Murs:
                  unique_limites = std::make_unique<LimitesMurs>();
      }
      return unique_limites;
}

