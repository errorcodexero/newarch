#ifndef AUTO_DISTANCES_H
#define AUTO_DISTANCES_H

#include "quick.h"
#include "unittype.h"
#include <iostream>

//Distances are in inches
#define AUTO_DISTANCES_ITEMS(X) \
	X(double,baseline) \
	X(double,middle_peg)

struct Auto_distances{
	STRUCT_MEMBERS(AUTO_DISTANCES_ITEMS)
	Auto_distances();
};

bool operator<(Auto_distances const&,Auto_distances const&);
bool operator==(Auto_distances const&,Auto_distances const&);
bool operator!=(Auto_distances const&,Auto_distances const&);
std::ostream& operator<<(std::ostream&,Auto_distances const&);

Auto_distances read_shooter_constants();
void write_shooter_constants(Auto_distances);

#endif
