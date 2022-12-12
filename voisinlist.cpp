#include "voisinlist.h" 

double getDet(const double* box)
{
  return box[0] * (box[4] * box[8] - box[5] * box[7]) +
         box[1] * (box[5] * box[6] - box[3] * box[8]) +
         box[2] * (box[3] * box[7] - box[4] * box[6]);
}

void getInverseBox(double* box)
{
  box[9] = box[4] * box[8] - box[5] * box[7];
  box[10] = box[2] * box[7] - box[1] * box[8];
  box[11] = box[1] * box[5] - box[2] * box[4];
  box[12] = box[5] * box[6] - box[3] * box[8];
  box[13] = box[0] * box[8] - box[2] * box[6];
  box[14] = box[2] * box[3] - box[0] * box[5];
  box[15] = box[3] * box[7] - box[4] * box[6];
  box[16] = box[1] * box[6] - box[0] * box[7];
  box[17] = box[0] * box[4] - box[1] * box[3];
  double det = getDet(box);
  for (int n = 9; n < 18; ++n) {
    box[n] /= det;
  }
}

void applyMicOne(double& x12)
{
  if (x12 < -0.5)
    x12 += 1.0;
  else if (x12 > +0.5)
    x12 -= 1.0;
}

void applyMic(const double* box, double& x12, double& y12, double& z12)
{
  double sx12 = box[9] * x12 + box[10] * y12 + box[11] * z12;
  double sy12 = box[12] * x12 + box[13] * y12 + box[14] * z12;
  double sz12 = box[15] * x12 + box[16] * y12 + box[17] * z12;
  applyMicOne(sx12);
  applyMicOne(sy12);
  applyMicOne(sz12);
  x12 = box[0] * sx12 + box[1] * sy12 + box[2] * sz12;
  y12 = box[3] * sx12 + box[4] * sy12 + box[5] * sz12;
  z12 = box[6] * sx12 + box[7] * sy12 + box[8] * sz12;
}

void findNeighbor(Atom& atom)
{
  const double cutoffSquare = atom.cutoffNeighbor * atom.cutoffNeighbor;
  std::fill(atom.NN.begin(), atom.NN.end(), 0);   //Number of neighbors of each atom set to zero

  for (int i = 0; i < atom.number - 1; ++i) {
    const double x1 = atom.x[i];
    const double y1 = atom.y[i];
    const double z1 = atom.z[i];
    for (int j = i + 1; j < atom.number; ++j) {
      double xij = atom.x[j] - x1;
      double yij = atom.y[j] - y1;
      double zij = atom.z[j] - z1;
      applyMic(atom.box, xij, yij, zij);
      const double distanceSquare = xij * xij + yij * yij + zij * zij;
      if (distanceSquare < cutoffSquare) {
        atom.NL[i * atom.MN + atom.NN[i]] = j;
        atom.NN[i]++;
        if (atom.NN[i] > atom.MN) {
          std::cout << "Error: number of neighbors for atom " << i
                    << " exceeds " << atom.MN << std::endl;
          exit(1);
        }
      }
    }
  }
}

