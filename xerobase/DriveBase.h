#pragma once

#include <Subsystem.h>
#include <Robot.h>

namespace xero {
	namespace base {
		class DriveBase : public Subsystem {
		public:
			DriveBase(Robot &robot, const std::string &name) : Subsystem(robot, name) {
			}

			virtual ~DriveBase() {
			}
		} ;
	}
}