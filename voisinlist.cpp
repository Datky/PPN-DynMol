
#include "voisinlist.h" 


// Struct to represent a pair of particle indices and their distance

struct ParticulePair {
	int i, j;
	double distance;
};

// Function to create the nearest neighbor table

std::vector<ParticulePair> makeNearestNeighborTable(Particules & particules) {
  

// Get pointers to the particle positions, velocities, and accelerations
  
  
	struct Vecteur_3D *__restrict positions = particules.pos;
	struct Vecteur_3D *__restrict velocities = particules.vit;
	struct Vecteur_3D *__restrict accelerations = particules.acc;


// Create an empty vector to hold the nearest neighbor table
  
  std::vector<ParticulePair> nearestNeighbors;

// Loop over all pairs of particles
  
 for (int i = 0; i < N; ++i) {
		for (int j = i + 1; j < N; ++j){
            
  // Calculate the distance between the two particles          
            const double dx = pos->X[i] - pos->X[j];
            const double dy = pos->Y[i] - pos->Y[j];
            const double dz = pos->Z[i] - pos->Z[j];
            const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            
 // Add the particle pair to the nearest neighbor table           
            
            nnearestNeighbors.push_back({i, j, distance}); }}
	
	// Sort the nearest neighbor table in ascending order of distance
	
	std::sort(nearestNeighbors.begin(), nearestNeighbors.end(), [](const ParticlePair& a, const ParticlePair& b) { 
		
		return a.distance < b.distance; });
	
	// Return the nearest neighbor table
	
	return nearestNeighbors;
}

				


// Sort the nearest neighbor table in ascending order of distance    
// std::sort(nearestNeighbors.begin(), nearestNeighbors.end(), [](const ParticulePair& a, const ParticulePair& b) { return a.distance < b.distance; });
// Return the nearest neighbor table return nearestNeighbors; }
  
  
