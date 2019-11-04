#pragma once

//
// A module is defined by a single bit in a 64 bit number.  The user of this
// library has the lower 32 bits to define modules.  The upper 32 bits are reserved
// for library modules
//

#define MODULE_AUTOMODE						(1ull << 63)
#define MODULE_TankDrive					(1ull << 62)
#define MODULE_PATHFINDER					(1ull << 61)
#define MODULE_ROBOTBASE					(1ull << 60)

#define MODULE_ALL							(0xFFFFFFFFFFFFFFFFull)
