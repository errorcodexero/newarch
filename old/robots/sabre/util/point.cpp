#include "point.h"
#include<iostream>
#include<math.h>
#include<cassert>
#include"util.h"

using namespace std;

Pt::Pt():x(0),y(0),theta(0){}
Pt::Pt(double a,double b,double c):x(a),y(b),theta(c){}

Pt operator*(Pt p,double d){
	p.x*=d;
	p.y*=d;
	p.theta*=p.theta;
	return p;
}

Pt& operator+=(Pt& a,Pt b){
	a.x+=b.x;
	a.y+=b.y;
	a.theta+=b.theta;
	return a;
}

ostream& operator<<(ostream& o,Pt p){
	return o<<"Pt("<<p.x<<","<<p.y<<","<<p.theta<<")";
}

bool approx_equal(double a,double b){
	return fabs(a-b)<.001;
}

bool approx_equal(Pt a,Pt b){
	return approx_equal(a.x,b.x) && approx_equal(a.y,b.y) && approx_equal(a.theta,b.theta);
}

Pt shift_axes_clockwise(Pt p,Rad theta){
	Pt r;
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
		Pt a(1,0,0);
		Pt b(0,1,0);
		Pt out=shift_axes_clockwise(a,M_PI/2);
		assert(approx_equal(out,b));
	}
}
#endif
