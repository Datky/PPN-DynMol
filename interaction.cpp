/** @fichier
 * 
 **/
#include <cmath>
#include "interaction.hpp"
#include "potentiel.hpp"

//Pseudo code //
void Verlet(Particules & at, f64 const& dt, f64 const& r_cut, f64 const& m){

      f64 r_globale;
      struct Vecteur_3D *__restrict r_tmp;
      struct Vecteur_3D *__restrict r;
      struct Vecteur_3D *__restrict F;

      r_tmp = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
      r = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
      F = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));

      r_tmp->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      r_tmp->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      r_tmp->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

      r->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      r->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      r->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

      F->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      F->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
      F->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

      u32 n = at.pos.X.size();

      for(u32 i=0; i<n; ++i){

            //Mise à zéro de la force
            F.X = F.Y = F.Z = 0;

            for(u32 j=0; j<n; ++j){

                  //Calcul de la distance entre les atome r_i(t+dt)
                  r_tmp.X = at.pos.X[i] - at.pos.X[j];
                  r_tmp.Y = at.pos.Y[i] - at.pos.Y[j];
                  r_tmp.Z = at.pos.Z[i] - at.pos.Z[j];
            

                  //Calcul des positions : p_i(t+dt)
                  at.pos.X[i] += at.v.X[i]*dt + 0.5*at.acc.X[i]*pow(dt,2.0);
                  at.pos.Y[i] += at.v.Y[i]*dt + 0.5*at.acc.Y[i]*pow(dt,2.0);
                  at.pos.Z[i] += at.v.Z[i]*dt + 0.5*at.acc.Z[i]*pow(dt,2.0);

                  //1er calcule des vitesses : v_i(t+dt/2)
                  at.vit.X[i] += 0.5*at.acc.X[i]*dt;
                  at.vit.Y[i] += 0.5*at.acc.Y[i]*dt;
                  at.vit.Z[i] += 0.5*at.acc.Z[i]*dt;

                  //Calcul de la force : F_i(t+dt) et a_i(t+dt)


                  r.X = at.pos.X[i] - at.pos.X[j];
                  r.Y = at.pos.Y[i] - at.pos.Y[j];
                  r.Z = at.pos.Z[i] - at.pos.Z[j];
                  r = sqrt( pow(r.X,2.0) + pow(r.Y,2.0) + pow(r.Z,2.0) );

                  if(r>r_cut) {
                        continue;
                  } 
                  else {
                        F.X += -(Lennard_Jones(r_tmp.X) - Lennard_Jones(r.X)) / r_tmp.X - r.X;
                        F.Y += -(Lennard_Jones(r_tmp.Y) - Lennard_Jones(r.Y)) / r_tmp.Y - r.Y;
                        F.Z += -(Lennard_Jones(r_tmp.Z) - Lennard_Jones(r.Z)) / r_tmp.Z - r.Z;
                  }


                  //Stocker plus grand déplacement dans ce pas de temps. f64 r_max
                  //if(r_max<r) r_max=r;

            }

            //Calcul des acceélération : a_i(t+dt)
            at.acc.X[i] = F.X/at.m;
            at.acc.Y[i] = F.Y/at.m;
            at.acc.Z[i] = F.Z/at.m;

            //2ième calcul de des vitesses : v_i(t+dt)
            at.vit.X[i] += 0.5*at.acc.X[i]*dt;
            at.vit.Y[i] += 0.5*at.acc.Y[i]*dt;
            at.vit.Z[i] += 0.5*at.acc.Z[i]*dt;

      }
      
}
