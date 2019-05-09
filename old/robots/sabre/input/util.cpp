#include "util.h"
#include<iostream>
#include<cassert>
#include<math.h>

using namespace std;

ostream& operator<<(ostream& o,Joystick_section j){
	switch(j){
		#define X(name) case Joystick_section::name: return o<<""#name;
		X(LEFT)
		X(RIGHT)
		X(UP)
		X(DOWN)
		X(CENTER)
		#undef X
		default: assert(0); break;
	}
	return o;
}

ostream& operator<<(ostream& o,POV_section p){
	switch(p){
		#define X(NAME) case POV_section::NAME: return o<<""#NAME;
		POV_SECTIONS
		#undef X
		default: assert(0); break;
	}
	return o;
}

Joystick_section joystick_section(double x,double y){
	static const double LIM=.25;
	if(fabs(x)<LIM && fabs(y)<LIM){
		return Joystick_section::CENTER;
	}
	if(x<y){
		if(x>-y){
			return Joystick_section::DOWN;
		}
		return Joystick_section::LEFT;
	}
	if(x>-y) return Joystick_section::RIGHT;
	return Joystick_section::UP;
}

Joystick_section divide_vertical(double y){ return joystick_section(0,y); }

POV_section pov_section(int x){
	switch(x){
		case 0: return POV_section::UP;
		case 45: return POV_section::UP_RIGHT;
		case 90: return POV_section::RIGHT;
		case 135: return POV_section::DOWN_RIGHT;
		case 180: return POV_section::DOWN;
		case 225: return POV_section::DOWN_LEFT;
		case 270: return POV_section::LEFT;
		case 315: return POV_section::UP_LEFT;
		default: return POV_section::CENTER;

	}
}

unsigned interpret_10_turn_pot(Volt v){
	/*array<Volt,10> limits={//for software development laptop use
		
		
		
	};*/
	
	array<Volt,10> limits={//for red and gold driverstation use
		-1.00,
		-0.75,
		-0.50,
		-0.25,
		0.00,
		0.20,
		0.40,
		0.60,
		0.80,
		1.00
	};
	for(unsigned i=0;i<10;i++){
		if(v<(limits[i] + 0.05)) return (i!=9) ? (i+1) : 0;
	}
	return 9;
}

#ifdef INPUT_UTIL_TEST
void joystick_section_test(){
	assert(joystick_section(0,0)==Joystick_section::CENTER);
	assert(joystick_section(-1,0)==Joystick_section::LEFT);
	assert(joystick_section(1,0)==Joystick_section::RIGHT);
	assert(joystick_section(0,-1)==Joystick_section::UP);
	assert(joystick_section(0,1)==Joystick_section::DOWN);
}

int main(){
	joystick_section_test();
}
#endif
