/** @fichier
 * 
 **/
#include <cmath>
#include "interaction.h"
#include "potentiel.cpp"
//#include "voisinlist.cpp"

//Pseudo code //
void Verlet(Particules & at, f64 const& dt, f64 const& r_cut){

      f64 F_x, F_y, F_z ;
      //f64 r_cut2 = r_cut*r_cut ;

      for(u32 i=0; i<N; ++i){

            //Mise à zéro de la force ///modif
            F_x = F_y = F_z = 0;
            

             //Calcul des positions : p_i(t+dt)
             at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
             at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
             at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

/*
             ////Limites spatiales periodiques avec origine en (b_x/2,b_y/2,b_z/2)
             while (at.pos->X[i]<-b_x/2) {
                  at.pos->X[i] += b_x/2;
             } 
             while (at.pos->X[i]>b_x/2) {
                  at.pos->X[i] -= b_x/2;
             }

             while (at.pos->Y[i]<-b_y/2) {
                  at.pos->Y[i] += b_y/2;
             } 
             while (at.pos->Y[i] > b_y/2) {
                  at.pos->Y[i] -= b_y/2;
             }
            
             while (at.pos->Z[i]<-b_z/2) {
                  at.pos->Z[i] += b_z/2;
             } 
             while (at.pos->Z[i]>b_z/2) {
                  at.pos->Z[i] -= b_z/2;
             }
*/
/*
             //Limites spatiales periodiques avec origine en (0,0,0) et nouvelle périodicité à 2,5*d des bords
             while (at.pos->X[i]<(0+2.5*d) {
                  at.pos->X[i] += b_x;
             }

             while (at.pos->X[i]>(b_x-2.5*d)) {
                  at.pos->X[i] -= b_x;
             } 

             while (at.pos->Y[i]<(0+2.5*d)) {
                  at.pos->Y[i] += b_y;
             }
             while (at.pos->Y[i]>(b_y-2.5*d)) {
                  at.pos->Y[i] -= b_y;
             }              
             while (at.pos->Z[i]<(0+2.5*d)) {
                  at.pos->Z[i] += b_z;
             }
             while (at.pos->Z[i]>(b_z-2.5*d)) {
                  at.pos->Z[i] -= b_z;
             } 
*/

             //Limites spatiales periodiques avec origine en (0,0,0) et nouvelle périodicité à 2,5*d des bords
             if (at.pos->X[i]<(0+2.5*d)) {
                  at.pos->X[i] += b_x;
             } else if (at.pos->X[i]>(b_x-2.5*d)) {
                  at.pos->X[i] -= b_x;
             } 

             if (at.pos->Y[i]<(0+2.5*d)) {
                  at.pos->Y[i] += b_y;
             } else if (at.pos->Y[i]>(b_y-2.5*d)) {
                  at.pos->Y[i] -= b_y;
             } 
             
             if (at.pos->Z[i]<(0+2.5*d)) {
                  at.pos->Z[i] += b_z;
             } else if (at.pos->Z[i]>(b_z-2.5*d)) {
                  at.pos->Z[i] -= b_z;
             } 

             //1er calcul des vitesses : v_i(t+dt/2)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

             //Calcul de la force : F_i(t+dt) et a_i(t+dt)
             for(u32 j=0; j<N; ++j){

                  //Calcul de la distance entre les atome r_i(t+dt)
                  f64 r_x = at.pos->X[i] - at.pos->X[j];
                  f64 r_y = at.pos->Y[i] - at.pos->Y[j];
                  f64 r_z = at.pos->Z[i] - at.pos->Z[j];
                  //f64 r_globale2 = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);
                  f64 r_globale = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));

                  //Calcul de la force si la distance inter-atomique globale est inférieure au rayon de coupure
                  if (r_globale<r_cut) {
                        F_x += F_Lennard_Jones(r_x);
                        F_y += F_Lennard_Jones(r_y);
                        F_z += F_Lennard_Jones(r_z);

                        //Stocker dans la liste de voisin
                        
                        //nearestVoisin();
                        
                        //at.liste[i].append(j)
                  }
                  //Stocker plus grand déplacement dans ce pas de temps. f64 r_max
                   
                   
                  //if(r_max<r_globale) r_max=r_globale;

             }

             //Calcul des accélérations : a_i(t+dt)
             at.acc->X[i] = F_x/m;
             at.acc->Y[i] = F_y/m;
             at.acc->Z[i] = F_z/m;

             //2ième calcul des vitesses : v_i(t+dt)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

      }
      
}
