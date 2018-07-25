#pragma once

#include "Subsystem.h"

namespace xero {
	namespace base {
		
		/// \brief the drivebase for the robot
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
			
			/// \brief returns the net distance travled in inches by the left wheels on the robot
			/// If the robot travels forward and then back by the same distance, the net distance
			/// is zero and zero will be returned.
			/// \returns the distance traveled in inches
			double getDistL() const {
				return dist_l_ ;
			}
			
			/// \brief returns the net distance travled in inches by the right wheels on the robot
			/// If the robot travels forward and then back by the same distance, the net distance
			/// is zero and zero will be returned.
			/// \returns the distance traveled in inches			
			double getDistR() const {
				return dist_r_ ;
			}
			
			/// \brief return the current angle of the robot relative to its starting angle
			/// \returns the current angle of the robot
			double getAngle() const {
				return angle_ ;
			}

			/// \brief compute the current state of the robot.
			/// This method generally reads the input sensors assocaited with the drivebase and
			/// calculates the current state of the robot including the distance traveled, the
			/// current angle of the robot, the speed and acceleration of the robot.  This method
			/// is generally called once per robot loop before any robot controller gets a chance
			/// to run.
			virtual void computeState() {
			}

			/// \brief run the drivebase subsystem
			/// This method is generally called after the robot controller gets a chance to assign
			/// directives to subsystems.  This method looks at any assigned subsytsem and the current
			/// state of the robot and decides what outputs to set to achieve the desired directive.  If
			/// no directive is assigned, the robot stops and remains still.
			// QUESTION: Is there not a better name than run() ????
			virtual void run() {
			}

		private:
			double dist_l_, dist_r_;
			double angle_;
		} ;
	}
}
