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

			virtual double getDist() const = 0 ;
			virtual double getAngle() const = 0 ;
			virtual double getVelocity() const = 0 ;
		} ;
	}
}