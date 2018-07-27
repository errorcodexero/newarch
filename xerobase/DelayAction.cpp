#include "DelayAction.h"
using namespace xero::base;

DelayAction::DelayAction() {
}

void DelayAction::setDelayTime(double delayTime) {
	delayTime_ = delayTime;
}

void DelayAction::start() {
	startTime_ = frc::Timer::GetFPGATimestamp();
	isDone_ = false;
	//timer_.Reset();
}

void DelayAction::run() {
	//timer_.Start();
}

bool DelayAction::isDone() {
	double elapsed = (frc::Timer::GetFPGATimestamp()-startTime_)*1000.0;
	if (elapsed >= delayTime_) {
		isDone_ = true;
	}
	return isDone_;
}

void DelayAction::end() {

}

bool DelayAction::cancel() {
	isDone_ = true;
	return true;
}

std::string DelayAction::toString() {

}



