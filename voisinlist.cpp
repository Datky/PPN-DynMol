
#include "voisinlist.h"


using namespace std;
using Particules = struct Particules;

struct Particules{

	int i, j;

	double r_globale;

};


void  Voisin(Particules& at, f64 const& r_cut) {

	struct Particules particules;

	std::vector<Particules> makeVoisinlist(Particules & particules);

		const Vecteur_3D* __restrict positions = particules.pos;
		const Vecteur_3D* __restrict velocities = particules.vit;
		const Vecteur_3D* __restrict accelerations = particules.acc;

		std::vector<Particules> nearestVoisin;

		for (int i = 0; i < N; ++i) {
			for (int j = i + 1; j < N; ++j) {

				// Calculate the distance between the two particles          
				const double r_x = abs(at.pos->X[j] - at.pos->X[i]);
				const double r_y = abs(at.pos->Y[j] - at.pos->Y[i]);
				const double r_z = abs(at.pos->Z[j] - at.pos->Z[i]);
				
				const double r_globale = sqrt(pow(r_x, 2.0) + pow(r_y, 2.0) + pow(r_z, 2.0));
     
				nearestVoisin.push_back({ i, j, r_globale });
			}
		}

		// Sort the nearest neighbor table in ascending order of distance

		std::sort(nearestVoisin.begin(), nearestVoisin.end(), [](const Particules& a, const Particules& b) {

			return a.r_globale < b.r_globale;

			}

		// Return the nearest neighbor table

		return nearestVoisin;
	}

  
