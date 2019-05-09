#ifndef POINT_H
#define POINT_H

#include<iosfwd>
#include "interface.h"

struct Pt{
	double x,y;//feet
	double theta;//radians

	Pt();
	Pt(double,double,double);
};

std::ostream& operator<<(std::ostream&,Pt);
Pt operator*(Pt,double);
Pt& operator+=(Pt&,Pt);

bool approx_equal(double,double);
bool approx_equal(Pt,Pt);

Pt shift_axes_clockwise(Pt p,Rad theta);

#endif
