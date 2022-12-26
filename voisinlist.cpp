
#include "voisinlist.h"


void  Voisin (Particles & at, f64 const & dt, f64 const & r_cut){



// Function to create the nearest neighbor table
	
	std::vector<Particles> makeVoisinlist(Particules & particules) {
  

// Get pointers to the particle positions, velocities, and accelerations
		
		struct Vecteur_3D *__restrict positions = particules.pos;
		struct Vecteur_3D *__restrict velocities = particules.vit;
		struct Vecteur_3D *__restrict accelerations = particules.acc;


// Create an empty vector to hold the nearest neighbor table
		
		std::vector<Particles> nearestVoisin;

// Loop over all pairs of particles
		
		for (int i = 0; i < N; ++i) {
			for (int j = i + 1; j < N; ++j){
            
  // Calculate the distance between the two particles          
				f64 r_x = at.pos->X[i] - at.pos->X[j];
				f64 r_y = at.pos->Y[i] - at.pos->Y[j];
				f64 r_z = at.pos->Z[i] - at.pos->Z[j];
				f64 r_globale = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0));
                  
                 
 // Add the particle pair to the nearest neighbor table           
				nearestVoisin.push_back({i, j, r_globale}); }}
	
	// Sort the nearest neighbor table in ascending order of distance
		
		std::sort(nearestVoisin.begin(), nearestVoisin.end(), [](const Particles& a, const Particles& b) { 
			
			return a.r_globale < b.r_globale; });
	
	// Return the nearest neighbor table
		
		return nearestVoisin;
	}

}			
  
  
