/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include "XYZ.h"
#include "SoA/particule.h"
#include "types.h"

//
void lireXYZ(std::string source, struct Vecteur_3D* pos) {


      std::ifstream fichier(source);

      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }


      // Récupère et saute les deux premières lignes inutiles.
      std::string s;
      std::getline(fichier, s);                      
      std::getline(fichier, s);

      f64 p_x, p_y, p_z;

      u64 i = 0;

      while (fichier >> s >> p_x >> p_y >> p_z) { //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            pos->X[i] = p_x;
            pos->Y[i] = p_y;
            pos->Z[i] = p_z;
            ++i;
      }


      fichier.close();
}

//
/*
void  ecrireXYZ(Position const &p, std::string cible){
      std::ofstream fichier;
      fichier.open(cible);

      fichier << p.x.size() << std::endl;                              //Ecriture du <Nombre d'atomes> 
 

      fichier << "Commentaire : " << std::endl;                        //Ecriture de la ligne de commentaire
 
      f64 p_x, p_y, p_z;
      std::string s = "Élément";

      for (int i = 0; i < p.x.size(); ++i) {
            p_x = p.x[i];
            p_y = p.y[i];
            p_z = p.z[i];
            fichier << s <<' '<< p_x <<' '<<' '<< p_y <<' '<< p_z;     //Ecriture des <Élément i> <x(i)> <y(i)> <z(i)>
            fichier << std::endl;
      }
      
      fichier.close();
}
*/