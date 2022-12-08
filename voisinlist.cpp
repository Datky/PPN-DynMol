/*

Number of nearest neighbors per atom

The whole box is divided into a number of cells.
The edge length of each cell is not less than the cutoff distance of the vosin table.

How many cells are divided into? 

Number of cells in each cell vector direction?



*/

/*
  for (int i = 0; i < atom.number - 1; ++i) {
    const double x1 = atom.x[i];
    const double y1 = atom.y[i];
    const double z1 = atom.z[i];
    for (int j = i + 1; j < atom.number; ++j) {
      double xij = atom.x[j] - x1;
      double yij = atom.y[j] - y1;
      double zij = atom.z[j] - z1;
      
      ——————> xij * xij + yij * yij + zij * zij;
      
      if ()
      ———————>atom.NL[i * atom.MN + atom.NN[i]++] = j;
      
        if (atom.NN[i] > atom.MN) {
          */


float getArea(const double* a, const double* b)
{
  const double s1 = ....;
  const double s2 = ....;
  const double s3 = ....;
  return sqrt(s1 * s1 + s2 * s2 + s3 * s3);
}

/*

....

*/


void findCell( const double* box, const double* r, const int* numCells, int* cell)
{
  double s[3]; 
  
  //Three vector directions
 
  ...
    
  for (int d = 0; d < 3; ++d) {
    cell[d] = ....;
    
    if (cell[d] < 0)
      cell[d] += numCells[d];
    
    if (cell[d] >= numCells[d])
      cell[d] -= numCells[d];
  }
  cell[3] = cell[0] + numCells[0] * (cell[1] + numCells[1] * cell[2]);
}



