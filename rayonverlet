
#include <iostream>
#include "main.cpp"



void rayonverlet1(const double Length, const double halfLength, double& x12)
{
  if (x12 < -halfLength)
    x12 += length;
  else if (x12 > +halfLength)
    x12 -= length;
}

void rayonverlet(const double b_x, const double b_y, const double b_z, double& x12, double& y12, double& z12)
{
    const double halfx = b_x * 0.5;
    const double halfy = b_y * 0.5;
    const double halfz = b_z * 0.5;
    rayonverlet1(b_x, halfx, x12);
    rayonverlet1(b_y, halfy, y12);
    rayonverlet1(b_z, halfz, z12);
}
