#include "../Headers/constantes.h"
#include "../Headers/interaction.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>



// Fixer le nombre de voisins de l'atome de fer i à NNi
//Définir le nombre maximum de voisins les plus proches
/*using namespace std;
extern u32 N;


*/
extern u32 MN; 

f64 rayonverlet1(f64 const& boxLength, f64 const& halfLength, f64 xij)
{
        if (xij < -halfLength)
                xij += boxLength;
        else if (xij > +halfLength)
                xij -= boxLength;
        return xij;
}


void rayonverlet(f64 bx, f64 by, f64 bz, f64& x12, f64& y12, f64& z12)
{
        f64 halfx = bx * 0.5;
        f64 halfy = by * 0.5;
        f64 halfz = bz * 0.5;
        x12 = rayonverlet1(bx, halfx, x12);
        y12 = rayonverlet1(by, halfy, y12);
        z12 = rayonverlet1(bz, halfz, z12);
        }



void Voisin(Particules & p, f64 r_cut,f64 r_skin, f64 b_x, f64 b_y, f64 b_z, std::vector<u32>& NN, std::vector<u32>& NL){


	f64 r_cut_verlet = r_cut + r_skin;
	f64 r_cut_verlet2 = r_cut_verlet * r_cut_verlet;
//    std::vector<u32> NN, NL;
//    std::vector<u32> NN(N,0);
//    std::vector<u32> NL(N*MN,0);	

    
    std::fill(NN.begin(), NN.end(), 0);

		for (u32 i = 0; i < N-1; ++i) {
			for (u32 j = i + 1; j < N; ++j) {

				// Calculate the distance between the two particles          
				f64 r_x = abs(p.pos->X[j] - p.pos->X[i]);
				f64 r_y = abs(p.pos->Y[j] - p.pos->Y[i]);
				f64 r_z = abs(p.pos->Z[j] - p.pos->Z[i]);
				rayonverlet(b_x, b_y, b_z, r_x, r_y, r_z);

				f64 r_global = r_x * r_x + r_y * r_y + r_z * r_z;
				if (r_global < r_cut_verlet2) {

					NL[i * MN + NN[i]++] = j;
					if (NN[i] > MN) {
						//std::cout << "Erreur : nombre de voisins pour l'atome " << i << " exceeds " << MN << std::endl;
						//exit(1);
						std::string error_msg = "Erreur : nombre de voisins pour l'atome " + std::to_string(i) + " exceeds " + std::to_string(MN);
                    throw std::runtime_error(error_msg);

					}

					
					}
   
			}
		}
}



