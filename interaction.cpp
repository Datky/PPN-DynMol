/** @fichier
 * 
 **/
#include <cmath>
#include "interaction.h"
#include "potentiel.cpp"

//Pseudo code //
void Verlet(Particules & at, Vecteur_3D* r_tmp, Vecteur_3D* r, Vecteur_3D* F, f64 const& dt, f64 const& r_cut){

      f64 r_globale;
      f64 h = pow(10,-9);

      for(u32 i=0; i<N; ++i){

            //Mise à zéro des distance inter-atomique précédente et actuelle et la force
            r_tmp->X[i] = r_tmp->Y[i] = r_tmp->Z[i] = 0;
            r->X[i] = r->Y[i] = r->Z[i] = 0;
            F->X[i] = F->Y[i] = F->Z[i] = 0;

            for(u32 j=0; j<N; ++j){

                  //Calcul de la distance entre les atome r_i(t+dt)
                  r_tmp->X[i] = at.pos->X[i] - at.pos->X[j];
                  r_tmp->Y[i] = at.pos->Y[i] - at.pos->Y[j];
                  r_tmp->Z[i] = at.pos->Z[i] - at.pos->Z[j];
            

                  //Calcul des positions : p_i(t+dt)
                  at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
                  at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
                  at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

                  //1er calcule des vitesses : v_i(t+dt/2)
                  at.vit->X[i] += 0.5*at.acc->X[i]*dt;
                  at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
                  at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

                  //Calcul de la force : F_i(t+dt) et a_i(t+dt)


                  r->X[i] = at.pos->X[i] - at.pos->X[j];
                  r->Y[i] = at.pos->Y[i] - at.pos->Y[j];
                  r->Z[i] = at.pos->Z[i] - at.pos->Z[j];
                  r_globale = sqrt( pow(r->X[i],2.0) + pow(r->Y[i],2.0) + pow(r->Z[i],2.0) );

                  //Calcule la force si la distance inter-atomique globale est inferieur au rayon de coupure
                  if (r_globale<r_cut) {
                        /*
                        F->X[i] += -(Lennard_Jones(r_tmp->X[i]) - Lennard_Jones(r->X[i])) / (r_tmp->X[i] - r->X[i]);
                        F->Y[i] += -(Lennard_Jones(r_tmp->Y[i]) - Lennard_Jones(r->Y[i])) / (r_tmp->Y[i] - r->Y[i]);
                        F->Z[i] += -(Lennard_Jones(r_tmp->Z[i]) - Lennard_Jones(r->Z[i])) / (r_tmp->Z[i] - r->Z[i]);
                        */
                        F->X[i] += -(Lennard_Jones(r->X[i]+h) - Lennard_Jones(h)) / h;
                        F->Y[i] += -(Lennard_Jones(r->Y[i]+h) - Lennard_Jones(h)) / h;
                        F->Z[i] += -(Lennard_Jones(r->Z[i]+h) - Lennard_Jones(h)) / h;
                  }


                  //Stocker plus grand déplacement dans ce pas de temps. f64 r_max
                  //if(r_max<r_globale) r_max=r_globale;

            }

            //Calcul des acceélération : a_i(t+dt)
            at.acc->X[i] = F->X[i]/m;
            at.acc->Y[i] = F->Y[i]/m;
            at.acc->Z[i] = F->Z[i]/m;

            //2ième calcul de des vitesses : v_i(t+dt)
            at.vit->X[i] += 0.5*at.acc->X[i]*dt;
            at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
            at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

            if (at.pos->X[i]<0) {
                  at.pos->X[i] = b_x;
            } else if (at.pos->X[i]>b_x) {
                  at.pos->X[i] = 0;
            }

            if (at.pos->Y[i]<0) {
                  at.pos->Y[i] = b_y;
            } else if (at.pos->Y[i]>b_y) {
                  at.pos->Y[i] = 0;
            }

            if (at.pos->Z[i]<0) {
                  at.pos->Z[i] = b_z;
            } else if (at.pos->Z[i]>b_z) {
                  at.pos->Z[i] = 0;
            }

      }
      
}
