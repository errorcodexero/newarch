#pragma once

#include "unittype.h"
#include <iosfwd>

struct Point{
    double x,y;//feet
    double theta;//radians

    Point();
    Point(double,double,double);
};

std::ostream& operator<<(std::ostream&,Point const&);
Point operator*(Point,double);
Point& operator+=(Point&,Point);

bool approx_equal(Point,Point);

Point shift_axes_clockwise(Point p,Rad theta);

