/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip> // Nécessaire pour bonne précision en lecture/écriture
#include "../Headers/XYZ_mpi.h"
#include "../Headers/constantes.h"

extern u32 N; // NEW
extern u32 nb_iteration; // NEW
extern u32 dt; // NEW


int lire_XYZ_Para(int b_z, int rang, int P, std::string source, struct Vecteur_Para &pos) {
      std::ifstream fichier(source);
      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }

      // Récupère et saute les deux premières lignes inutiles.
      std::string s;
      std::getline(fichier, s);                      
      std::getline(fichier, s);

      f64 p_x, p_y, p_z;


      int cellules_locales = c_z / P;
      int cellules_locales_P = cellules_locales;
      if (rang == P-1) {cellules_locales_P += c_z % P;}


      f64 taille_cellule = (f64)b_z/c_z;
      f64 frontiere_locale_deb = (taille_cellule*cellules_locales)*rang;
      f64 frontiere_locale_fin = frontiere_locale_deb+(taille_cellule*cellules_locales_P);


      int i = 0;
      while (fichier >> std::setprecision(11) >> s >> p_x >> p_y >> p_z) { //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            if (p_z >= frontiere_locale_deb && p_z < frontiere_locale_fin) {
                  pos.X.push_back(p_x);
                  pos.Y.push_back(p_y);
                  pos.Z.push_back(p_z);
                  ++i;
            }
      }
      
      fichier.close();
      return i;
}

void ecrire_XYZ_Para_local(std::string cible, std::vector<u32> &ids, struct Vecteur_Para &pos, int n_local) {
      std::ofstream fichier;
      fichier.open(cible);  
      
      for (int i = 0; i < n_local; ++i) {
            fichier << std::setprecision(11)<< ids[i] <<' '<< pos.X[i] <<' '<<' '<< pos.Y[i] <<' '<< pos.Z[i];   //Ecriture des <Identifiants i> <x(i)> <y(i)> <z(i)>
            fichier << std::endl;
      }
      
      fichier.close();
}

//
void lireXYZ(std::string source, struct Vecteur_3D* pos){
      std::ifstream fichier(source);

      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }


      // Récupère et saute les deux premières lignes inutiles.
      std::string s;
      std::getline(fichier, s);                      
      std::getline(fichier, s);

      f64 p_x, p_y, p_z;

      u64 i = 0;

      while (fichier >> std::setprecision(11) >> s >> p_x >> p_y >> p_z) { //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            pos->X[i] = p_x;
            pos->Y[i] = p_y;
            pos->Z[i] = p_z;
            ++i;
      }


      fichier.close();
}



//
void  ecrireXYZ(struct Vecteur_3D* pos, std::string cible){
      std::ofstream fichier;
      fichier.open(cible);

      fichier << N << std::endl;                              //Ecriture du <Nombre d'atomes> 
 

      fichier << "Commentaire : " << std::endl;                        //Ecriture de la ligne de commentaire
 
      
      std::string s = "Ar"; // NEW

      for (u32 i = 0; i < N; ++i) {
            fichier << std::setprecision(11)<< s <<' '<< pos->X[i] <<' '<<' '<< pos->Y[i] <<' '<< pos->Z[i];     //Ecriture des <Élément i> <x(i)> <y(i)> <z(i)>
            fichier << std::endl;
      }
      
      fichier.close();
}


