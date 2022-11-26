/** @fichier
 * 
 **/

#include <interaction.hpp>
#include <potentiel.hpp>

//Pseudo code //
void energie_atome(les_atomes & at, std::vector<f64> & E, f64 const& r_cut){
      f64 r;
      for(int i; i<N; ++i){
            for(int j; j<N; ++j){
                  r = at[i].Position-at[j].Position;
                  
                  if(r>r_cut) continue;
                  
                  else E[i] = Lennard-Jones(f64 r);
            }
      }
}
//Optimisation liste de voisin et optimiser le stocage des E.

//Pseudo code //
void Verlet(Atome & at, f64 const& dt){
      for(int i; i<N; ++i){
            //Calcul des positions : r_i(t+dt)
            r[i] += v[i]*dt + 0.5*a[i]*pow(dt,2.0);

            //1er calcule des vitesses : v_i(t+dt/2)
            v[i] += 0.5*a[i]*dt;

            //Calcul de la force : F_i(t+dt) et a_i(t+dt)
            F[i] = -(dE[i]/dr_i(t+dt));  //faire une fonction////////////
            a[i] = F[i]/m[i];

            //2ième calcul de des vitesses : v_i(t+dt)
            v[i] += 0.5*a[i]*dt;

            //Stocker plus grand déplacement dans ce pas de temps. f64 r_max
            //if(r_max<r[i]) r_max=r[i];
      }
      
}
