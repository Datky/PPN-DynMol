/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include "XYZ.hpp"


//
Vecteur_3D lireXYZ(std::string source){

      std::ifstream fichier;
      fichier.open(source);

      if (not fichier){ throw std::runtime_error("Fichier non trouve : "+source); }

      u32 nbAtome;
      fichier >> nbAtome;                            //Récupération du <Nombre d'atomes>

      Vecteur_3D p;
      p->X.reserve(nbAtome);                         //capacity=nbAtome
      p->Y.reserve(nbAtome);
      p->Z.reserve(nbAtome);

      std::string s;
      fichier >> s;
      std::getline(fichier, s);                      //Récupère et saute la ligne de commentaire

      f64 p_x, p_y, p_z;

      for (u32 i=0; i < nbAtome; ++i) {
            fichier >> s >> p_x >> p_y >> p_z;       //Récupération des <Élément i> <x(i)> <y(i)> <z(i)>
            p->X.push_back(p_x);
            p->Y.push_back(p_y);
            p->Z.push_back(p_z);
      }
      
      fichier.close();
      return p;
}

//
void  ecrireXYZ(Vecteur_3D const &p, std::string cible){

      std::ofstream fichier;
      fichier.open(cible, std::ios_base::app);

      u32 size = p->X.size()

      fichier << size << std::endl;                               //Ecriture du <Nombre d'atomes> 
 

      fichier << "Commentaire : - " << std::endl;                 //Ecriture de la ligne de commentaire
 
      f64 p_x, p_y, p_z;
      std::string s = "Ar";

      for (u32 i=0; i < size; ++i) {
            p_x = p->X[i];
            p_y = p->Y[i];
            p_z = p->Z[i];
            fichier << s <<"   "<< std::fixed << std::setprecision(5) << p_x <<"   "
                                << std::fixed << std::setprecision(5) << p_y <<"   "
                                << std::fixed << std::setprecision(5) << p_z;       //Ecriture des <Élément i> <x(i)> <y(i)> <z(i)>
            fichier << std::endl;
      }
      
      fichier.close();
}
