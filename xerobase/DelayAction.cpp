#include "DelayAction.h"
using namespace xero::base;

DelayAction::DelayAction() {
	block_ = true;
}

void DelayAction::setDelayTime(double delayTime) {
	delayTime_ = delayTime;
}

void DelayAction::start() {
	timer_.Reset();
}

void DelayAction::run() {
	timer_.Start();
}

bool DelayAction::isDone() {
	double elapsed = timer_.Get()*1000.0;
	if (elapsed >= delayTime_) {
		return true;
	}
	return false;
}

void DelayAction::end() {
	timer_.Stop();
}



