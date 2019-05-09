#ifndef TELEOP_H
#define TELEOP_H

#include "mode.h"

struct Teleop : Mode {
	unique_ptr<Mode> next_mode(Next_mode_info);
	Toplevel::Goal run(Run_info);
};

#endif
