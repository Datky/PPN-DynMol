#include "voisinlist.h" 



// Struct to represent a pair of particle indices and their distance

struct ParticulePair {
  int i, j;
  double distance;
};

// Function to create the nearest neighbor table

std::vector<ParticulePair> makeNearestNeighborTable(const Particules& particules) {
  

// Get pointers to the particle positions, velocities, and accelerations
  
  const Vecteur3D* __restrict positions = particules.pos;
  const Vecteur3D* __restrict velocities = particules.vit;
  const Vecteur3D* __restrict accelerations = particules.acc;


// Create an empty vector to hold the nearest neighbor table
  
  std::vector<ParticulePair> nearestNeighbors;

// Loop over all pairs of particles
  
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {

// Calculate the distance between the two particles
      
      const double dx = positions->X[i] - positions->X[j];
      const double dy = positions->Y[i] - positions->Y[j];
      const double dz = positions->Z[i] - positions->Z[j];
      const double distance = std::sqrt(dx * dx + dy * dy + dz * dz);


// Add the particle pair to the nearest neighbor table
      
      nearestNeigh 
        bors.push_back({i, j, distance}); } }


// Sort the nearest neighbor table in ascending order of distance    
// std::sort(nearestNeighbors.begin(), nearestNeighbors.end(), [](const ParticulePair& a, const ParticulePair& b) { return a.distance < b.distance; });
// Return the nearest neighbor table return nearestNeighbors; }
  
  
  
