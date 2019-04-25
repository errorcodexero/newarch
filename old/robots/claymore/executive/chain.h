#ifndef CHAIN_H
#define CHAIN_H

#include "executive.h"
#include "step.h"

struct Chain:Executive_impl<Chain>{
	unsigned current_step;
	std::vector<Step>* steps;
	Executive next;

	Chain(std::vector<Step>&, Executive);
	//Chain(Step&, Executive);

	Toplevel::Goal run(Run_info);
	Executive next_mode(Next_mode_info);
	bool operator==(Chain const&)const;
	bool operator<(Chain const&)const;
	std::unique_ptr<Executive_interface> clone()const;
	void display(std::ostream&)const;
};

#endif
