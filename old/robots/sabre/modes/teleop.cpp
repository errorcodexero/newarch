#include "teleop.h"

unique_ptr<Mode> Teleop::next_mode(Next_mode_info) {
	return make_unique<Teleop>();
}

Toplevel::Goal Teleop::run(Run_info) {
	return Toplevel::Goal{};
}

#ifdef TELEOP_TEST
int main() {
	
}
#endif
