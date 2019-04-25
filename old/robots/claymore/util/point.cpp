#include "point.h"
#include "util.h"
#include <iostream>
#include <math.h>
#include <cassert>


using namespace std;

Point::Point():x(0),y(0),theta(0){}
Point::Point(double a,double b,double c):x(a),y(b),theta(c){}

Point operator*(Point p,double d){
    p.x*=d;
    p.y*=d;
    p.theta*=p.theta;
    return p;
}

Point& operator+=(Point& a,Point b){
    a.x+=b.x;
    a.y+=b.y;
    a.theta+=b.theta;
    return a;
}

ostream& operator<<(ostream& o,Point const& p){
    return o<<"Point(x:"<<p.x<<" y:"<<p.y<<" theta:"<<p.theta<<")";
}

bool approx_equal(Point a,Point b){
    return approx_equal(a.x,b.x) && approx_equal(a.y,b.y) && approx_equal(a.theta,b.theta);
}

Point shift_axes_clockwise(Point p,Rad theta){
    Point r;
    r.x=cos(theta)*p.x+sin(theta)*p.y;
    r.y=sin(theta)*p.x+cos(theta)*p.y;
    r.theta=p.theta;
    return r;
}

#ifdef POINT_TEST

//thi is here for cygwin compatibility
#ifndef M_PI
#define M_PI 3.14159
#endif

int main(){
    {
	Point a(1,0,0);
	Point b(0,1,0);
	Point out=shift_axes_clockwise(a,M_PI/2);
	assert(approx_equal(out,b));
    }
}
#endif
