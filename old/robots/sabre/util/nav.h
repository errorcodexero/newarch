#ifndef NAV_H
#define NAV_H

#include <vector>
#define ROBOTSPACE (18/2)
///////////////////////////////////////////////////////////////////////////////
//enums
//
//the enums for detremine the orentation 
//////////////////////////////////////////////////////////////////////////////
enum Direction { LEFT=0, RIGHT=1, UP=2, DOWN=3};
enum Movedir { MFORWARD=0, MRIGHT=1, MLEFT=2, MBACK=3};

struct Pt2{
	int x, y;
};

std::vector<std::pair<int,Movedir>> solvemaze(Pt2,Pt2,Direction,Direction);

#endif

