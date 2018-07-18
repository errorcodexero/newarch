#pragma once

#include "Action.h"
#include <memory>

namespace xero {
	namespace base {
		
		//
		// Forward declaraion of the robot class.
		//
		class Robot ;
		
		/// \brief This is the base class for any subsystem in the system.
		class Subsystem {
		public:
			/// \brief create a new subsystem
			Subsystem(Robot &robot, const std::string &name) : robot_(robot) , m_name(name) {
			}

			/// \brief returns the name of the subsystem
			/// \returns the name of the subsystem
			const std::string &getName() const {
				return m_name ;
			}

			/// \brief compute the current state of the robot.
			/// The subsystem generally reads and associated input sensors and
			/// computes a state that is meaningful to users of the subsystem.

     		virtual void computeState() = 0 ;

			/// \brief set the current directive for the subsystem
			/// \param directive the new directive for the subsystem
			/// \return true if the directive is accepted, false if not
			virtual bool setDirective(std::shared_ptr<Action> directive) {
				directive_ = directive ;
				return true ;
			}

			/// \brief return a constant pointer to the current directive
			/// \returns  a constant pointer to the current directive
			const std::shared_ptr<Action> getDirective() const {
				return directive_ ;
			}

			/// \brief returns a pointer to the current directive
			/// \returns a pointer to the current directive
			std::shared_ptr<Action> getDirective()  {
				return directive_ ;
			}

			/// \brief set output actuators associated with the subsystem
			/// The output actuators are set to achieve the currently active
			/// directive based on the current state of the subsystem.
			virtual void run() = 0 ;

			/// \brief returns true if the subsystem is done with the current directive
			/// \returns true if the subsystem is done with the current directive
			virtual bool isDone() const {
				return directive_->isDone() ;
			}

		private:
			//
			// A reference to the robot object that contains this subsystem
			//
			Robot& robot_;

			//
			// The name of the subsystem (e.g. drivebase)
			//
			std::string m_name ;

			//
			// The currently active directive
			//
			std::shared_ptr<Action> directive_;
		} ;
	}
}
