/*

Number of nearest neighbors per atom

The whole box is divided into a number of cells.
The edge length of each cell is not less than the cutoff distance of the vosin table.

How many cells are divided into? 

Number of cells in each cell vector direction?



*/


float getArea(const double* a, const double* b)
{
  const double s1 = a[1] * b[2] - a[2] * b[1];
  const double s2 = a[2] * b[0] - a[0] * b[2];
  const double s3 = a[0] * b[1] - a[1] * b[0];
  return sqrt(s1 * s1 + s2 * s2 + s3 * s3);
}

/*

....

*/


void findCell( const double* box, const double* thickness, const double* r, const int* numCells, int* cell)
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



