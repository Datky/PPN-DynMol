/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include "XYZ.hpp"
#include "SoA/particule.h"

//
void lireXYZ(std::string source, struct Vecteur_3D* pos){
      std::ifstream fichier;
      fichier.open(source);

      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }

      u64 nbAtome;
      fichier >> nbAtome;                           //Récupération du <Nombre d'atomes>


      std::string s;
      std::getline(fichier, s);                      //Récupère et saute la ligne de commentaire

      f64 p_x, p_y, p_z;

      for (int i = 0; i < nbAtome; ++i) {
            fichier >> s >> p_x >> p_y >> p_z;     //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            pos->X[i] = p_x;
            pos->Y[i] = p_y;
            pos->Z[i] = p_z;
      }
      
      fichier.close();
}

//
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

int main(){ return 0; }
