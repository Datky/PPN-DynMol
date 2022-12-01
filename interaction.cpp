/** @fichier
 * 
 **/
#include <cmath>
#include "interaction.hpp"
#include "potentiel.hpp"

//Pseudo code //
/* void energie_atome(Atomes & at, std::vector<f64> & E, f64 const& r_cut){
      f64 r;
      for(int i; i<N; ++i){
            for(int j; j<N; ++j){
                  r = at.p[i]-at.p[j];
                  if(r>r_cut) continue;
                  else E[i][j] = Lennard-Jones(r);
            }
      }
}
//Optimisation liste de voisin et optimiser le stocage des E.
*/

//Pseudo code //
void Verlet(Atomes & at, f64 const& dt, f64 const& h, f64 const& r_cut){

      f64 r, r_x, r_y, r_z, F_x, F_y, F_z;
      u32 n = at.p.x.size();

      for(u32 i=0; i<n; ++i){
            //Calcul des positions : p_i(t+dt)
            at.p.x[i] += at.v.x[i]*dt + 0.5*at.a.x[i]*pow(dt,2.0);
            at.p.y[i] += at.v.y[i]*dt + 0.5*at.a.y[i]*pow(dt,2.0);
            at.p.z[i] += at.v.z[i]*dt + 0.5*at.a.z[i]*pow(dt,2.0);

            //1er calcule des vitesses : v_i(t+dt/2)
            at.v.x[i] += 0.5*at.a.x[i]*dt;
            at.v.y[i] += 0.5*at.a.y[i]*dt;
            at.v.z[i] += 0.5*at.a.z[i]*dt;

            //Calcul de la force : F_i(t+dt) et a_i(t+dt)
            for(u32 j=0; j<n; ++j){

                  r_x = at.p.x[i]-at.p.x[j];
                  r_y = at.p.y[i]-at.p.y[j];
                  r_z = at.p.z[i]-at.p.z[j];
                  r = sqrt(pow(r_x,2.0)+pow(r_y,2.0)+pow(r_z,2.0));

                  if(r>r_cut) {
                        continue;
                  } 
                  else {
                        F_x = -(Lennard_Jones(r_x+h) - Lennard_Jones(r_x))/h;
                        F_y = -(Lennard_Jones(r_y+h) - Lennard_Jones(r_y))/h;
                        F_z = -(Lennard_Jones(r_z+h) - Lennard_Jones(r_z))/h;
                  }

            }
            at.a.x[i] = F_x/at.m[i];
            at.a.y[i] = F_y/at.m[i];
            at.a.z[i] = F_z/at.m[i];

            //2ième calcul de des vitesses : v_i(t+dt)
            at.v.x[i] += 0.5*at.a.x[i]*dt;
            at.v.y[i] += 0.5*at.a.y[i]*dt;
            at.v.z[i] += 0.5*at.a.z[i]*dt;

            //Stocker plus grand déplacement dans ce pas de temps. f64 r_max
            //if(r_max<r[i]) r_max=r[i];
      }
      
}
