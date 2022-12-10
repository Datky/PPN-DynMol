/** @fichier
 * Lecture et écriture de fichier .xyz
 **/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip> // Nécessaire pour bonne précision en lecture/écriture
#include "XYZ.h"
#include "constantes.h"

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
      fichier.open(cible, std::ofstream::app);

      fichier << N << std::endl;                              //Ecriture du <Nombre d'atomes> 
 

      fichier << "Commentaire : " << std::endl;                        //Ecriture de la ligne de commentaire
 
      
      std::string s = "Élément";

      for (int i = 0; i < N; ++i) {
            fichier << std::setprecision(11)<< s <<' '<< pos->X[i] <<' '<<' '<< pos->Y[i] <<' '<< pos->Z[i];     //Ecriture des <Élément i> <x(i)> <y(i)> <z(i)>
            fichier << std::endl;
      }
      
      fichier.close();
      std::cout << "Bonne écriture sur le fichier." << std::endl;
}


