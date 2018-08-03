#include "StallMonitor.h"

namespace xero {
namespace misc {

StallMonitor::StallMonitor() {}

StallMonitor::StallMonitor(int samples, double threshold) {
	init(samples, threshold);
}

void StallMonitor::init(int samples, double threshold) {
	samples_ = samples;
	threshold_ = threshold;
}

void StallMonitor::initFromSettings(SettingsParser parser, std::string prefix) {
	samples_ = parser.getInteger(prefix + ":samples");
	threshold_ = parser.getDouble(prefix + ":threshold");
}

void StallMonitor::addSample(double sample) {
	history_.push_back(sample);
	if(history_.size() > samples_) {
		history_.pop_front();
	}
}

bool StallMonitor::isStalled() {
	double max = *max_element(history_.begin(), history_.end());
	double min = *min_element(history_.begin(), history_.end());
	return history_.size() == samples_ && (max - min) < threshold_;
}

void StallMonitor::reset() {
	history_.clear();
}

}
}
