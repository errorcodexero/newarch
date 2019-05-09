#ifndef NAV2_H
#define NAV2_H

#include<vector>
#include "nav.h"

struct Nav2{
	struct NavS{
		float left; 
		float right;
		float amount;
	};
	struct Navloadinput{
		Pt2 ptone;
		Pt2 pttwo;
		Direction dirone;
		Direction dirtwo;
	};
	
	struct Navinput{
		Pt2 navpt;
		Direction navdir;
	};
	struct Aturn{
		float l;
		float r;
		float dur;
		Aturn();
		Aturn(float,float,float);
	};

	int stepcounter;
	Navloadinput s1;
	Navloadinput s2;
	Navloadinput s3;
	Navloadinput s4;
	Navloadinput s5;
	
	Aturn Aturn;

	
	unsigned int navindex;
	std::vector<NavS> NavV;
		
	std::vector<NavS> loadnav(Navloadinput navin);
	
	Nav2();
};

#endif
