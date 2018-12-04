#pragma once

#include "ControllerBase.h"
#include "ActionSequence.h"
#include "OISubsystem.h"
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>
#include <cassert>

/// \file


namespace xero {
	namespace base {
		/// \brief This is the base class for any autonomous mode controller.
		/// This class contains the code to execute an automode program.
		/// A derived classs generally creates the auto program for a given
		/// robot, automode selector, and set of field conditions.  This class simply
		/// runs a single action.  For more complex automodes this single action
		/// should be an action sequence or parallel action.
		/// \sa ActionSequence
		/// \sa ParallelAction
		class AutoController : public ControllerBase {
		public:
			/// \brief Create a new automode controller object
			/// \param robot the robot this controller is associated with
			AutoController(Robot &robot):ControllerBase(robot) {				
			}

			/// \brief This method is called once per robot loop by the main robot class
			/// For the AutoController class this method calls the action that is the current
			/// automode that is running.
			virtual void run() {
				if (actionptr_ != nullptr) {
					actionptr_ ->run();

					if (actionptr_->isDone())
						actionptr_ = nullptr ;
				}
			}

			/// \brief return a human readable name for the auto mode
			/// \returns auto mode name
			virtual std::string getAutoModeName() {
				std::string ret = "None" ;

				if (actionptr_ != nullptr)
					ret = actionptr_->getName() ;

				return ret ;
			}


			/// \brief update the automode as the selector has changed
			/// \param sel the automode selector value
			virtual void update(int sel) = 0 ;

		protected:
			/// \brief sets the reference to the action that will be run when run() is called
			/// \param action run for a controller
			void setAction(ActionSequencePtr action) {
				actionptr_ = action;
				if (actionptr_ != nullptr)
					actionptr_->start() ;
			}


		private:
			ActionSequencePtr actionptr_;
		} ;
	}
}
