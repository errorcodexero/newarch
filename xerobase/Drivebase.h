#pragma once

#include "Subsystem.h"
#include <AHRS.h>

namespace xero {
	namespace base {
		/// \brief The drivebase for the robot
		class Drivebase : public Subsystem {
		public:
			/// \brief A directive for the drivebase.  All directives for the drivebase should be derived from this class.
			class DrivebaseDirective : public Action {} ;

			/// \brief Drives the drivebase at the given velocity
			class VelocityDirective : public DrivebaseDirective {};

			/// \brief Drives the drivebase straight for a given distance
			class DistanceDirective : public DrivebaseDirective {};

			/// \brief Create a new drivebase object
			/// \param robot The robot that contains this drivebase subsystem
			Drivebase(Robot& robot);
			
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

			/// \brief Set the directive for the drivebase
			/// \param directive The new directive for the drivebase
			/// \returns QUESTION: What should this return?
			virtual bool setDirective(std::shared_ptr<DrivebaseDirective> directive) ;

		private:
			AHRS navx_;

			double angle_;
		} ;
	}
}
