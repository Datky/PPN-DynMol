#include "main.cpp"
#include "constantes.h"
#include "rayonverlet.cpp"
#include "AoS/particule.h"
#include <cmath>
#include <vector>


// Fixer le nombre de voisins de l'atome de fer i à NNi
//Définir le nombre maximum de voisins les plus proches
std::vector<int> NN, NL;
const int MN = 10000;


void  Voisin(struct Particule* liste, f64 const& r_cut) {

	double r_cut2 = r_cut * r_cut;

	std::fill(NN.begin(), NN.end(), 0);

		for (int i = 0; i < N; ++i) {
			for (int j = i + 1; j < N; ++j) {

				// Calculate the distance between the two particles          
				double r_x = abs(liste[j].pos_X - liste[i].pos_X);
				double r_y = abs(liste[j].pos_Y - liste[i].pos_Y);
				double r_z = abs(liste[j].pos_Z - liste[i].pos_Z);
				rayonverlet(b_x, b_y, b_z, r_x, r_y, r_z);
				const double r_global = sqrt(pow(r_x, 2.0) + pow(r_y, 2.0) + pow(r_z, 2.0));

				if (r_global < r_cut && r_global != 0) {

					NL[i * MN + NN[i]++] = j;
					if (NN[i] > MN) {
						std::cout << "Erreur : nombre de voisins pour l'atome " << i << " exceeds " << MN << std::endl;
						exit(1);

					}

					
					}
   
			}
		}
}
