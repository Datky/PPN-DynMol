#include "constantes.h"
#include <iostream>
#include "main.cpp"
#include "types.h"

void rayonverlet1(const double Length, const double halfLength, double& xij)
{
	if (xij < -halfLength)
		xij += Length;
	else if (xij > +halfLength)
		xij -= Length;
}


void rayonverlet(const double bx, const double by, const double bz, double& x12, double& y12, double& z12)
{
	const double halfx = bx * 0.5;
	const double halfy = by * 0.5;
	const double halfz = bz * 0.5;
	rayonverlet1(b_x, halfx, x12);
	rayonverlet1(b_y, halfy, y12);
	rayonverlet1(b_z, halfz, z12);
}
