/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include "XYZ.hpp"


//
Vector_3D lireXYZ(std::string source){
      std::ifstream fichier;
      fichier.open(source);

      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }

      u32 nbAtome;
      fichier >> nbAtome;                           //Récupération du <Nombre d'atomes>
      Vector_3D p;
      p.x.reserve(nbAtome);                         //capacity=nbAtome
      p.y.reserve(nbAtome);
      p.z.reserve(nbAtome);

      std::string s;
      fichier >> s;
      std::getline(fichier, s);                      //Récupère et saute la ligne de commentaire

      f64 p_x, p_y, p_z;

      for (u32 i=0; i < nbAtome; ++i) {
            fichier >> s >> p_x >> p_y >> p_z;      //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            p.x.push_back(p_x);
            p.y.push_back(p_y);
            p.z.push_back(p_z);
      }
      
      fichier.close();
      return p;
}

//
void  ecrireXYZ(Vector_3D const &p, std::string cible){
      std::ofstream fichier;
      fichier.open(cible);

      fichier << p.x.size() << std::endl;                              //Ecriture du <Nombre d'atomes> 
 

      fichier << "Commentaire : - " << std::endl;                      //Ecriture de la ligne de commentaire
 
      f64 p_x, p_y, p_z;
      std::string s = "É";

      for (u32 i=0; i < p.x.size(); ++i) {
            p_x = p.x[i];
            p_y = p.y[i];
            p_z = p.z[i];
            fichier << s <<"   "<< std::fixed << std::setprecision(5) << p_x <<"   "
                               << std::fixed << std::setprecision(5) << p_y <<"   "
                               << std::fixed << std::setprecision(5) << p_z;       //Ecriture des <Élément i> <x(i)> <y(i)> <z(i)>
            fichier << std::endl;
      }
      
      fichier.close();
}
