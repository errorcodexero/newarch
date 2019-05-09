#ifndef MODE_H
#define MODE_H

#include <memory>
#include "../control/toplevel.h"

struct Next_mode_info {
	
};

struct Run_info {

};

struct Mode {
	virtual unique_ptr<Mode> next_mode(Next_mode_info)=0;
	virtual Toplevel::Goal run(Run_info)=0;
};

#endif
