/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include "XYZ.hpp"

//
Position lireXYZ(std::string source){
      std::ifstream fichier;
      fichier.open(source);

      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }

      u32 nbAtome;
      fichier >> nbAtome;                           //Récupération du <Nombre d'atomes>
      Position p;
      p.x.reserve(nbAtome);                         //capacity=nbAtome
      p.y.reserve(nbAtome);
      p.z.reserve(nbAtome);

      std::string s;
      std::getline(fichier, s);                      //Récupère et saute la ligne de commentaire

      f64 p_x, p_y, p_z;

      for (int i = 0; i < nbAtome; ++i) {
            fichier >> s >> p_x >> p_y >> p_z;     //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            p.x.push_back(p_x);
            p.y.push_back(p_y);
            p.z.push_back(p_z);
      }
      
      fichier.close();
      return p;
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
