#include "constantes.h"
#include <iostream>
//#include "main.cpp"
#include "types.h"


void rayonverlet1(const double boxLength, const double halfLength, double& xij)
{
        if (xij < -halfLength)
                xij += boxLength;
        else if (xij > +halfLength)
                xij -= boxLength;
}


void rayonverlet(const double bx, const double by, const double bz, double& x12, double& y12, double& z12)
{
        const double halfx = bx * 0.5;
        const double halfy = by * 0.5;
        const double halfz = bz * 0.5;
        rayonverlet1(b_x, halfx, x12);
        rayonverlet1(b_y, halfy, y12);
        }

