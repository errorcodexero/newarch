#pragma once

#include "Subsystem.h"
#include <AHRS.h>

namespace xero {
	namespace base {
		/// \brief The drivebase for the robot
		class Drivebase : public Subsystem {
			/// \brief This is a directive for the drivebase.  All directives for the drivebase should be derived form this class.
			class DrivebaseDirective : public Action {
			} ;
		public:
			/// \brief create a new drive base object
			/// \param robot the robot that contains this drivebase subsystem
			Drivebase(Robot& robot);

			/// \brief destroy the drivebase
			virtual ~Drivebase() ;
			
			/// \brief Return the current angle of the robot relative to its starting angle
			/// \returns The current angle of the robot
			double getAngle() const {
				return angle_ ;
			}

			/// \brief Compute the current state of the drivebase
			/// This method generally reads the input sensors associated with the drivebase and
			/// calculates the current state of the drivebase, namely the angle of the robot.
			/// This method is generally called once per robot loop before any robot controller gets a chance
			/// to run.
			virtual void computeState();

			/// \brief Run the drivebase subsystem
			/// This method is generally called after the robot controller gets a chance to assign
			/// directives to subsystems.  This method looks at any assigned subsytsem and the current
			/// state of the robot and decides what outputs to set to achieve the desired directive.  If
			/// no directive is assigned, the robot stops and remains still.
			// QUESTION: Is there not a better name than run() ????
			virtual void run() {
			}

		private:
			AHRS navx_;

			double angle_;
		} ;
	}
}
