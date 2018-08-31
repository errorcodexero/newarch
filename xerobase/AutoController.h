#pragma once

#include "ControllerBase.h"
#include "Action.h"

namespace xero {
	namespace base {
		/// \brief This is the base class for any autonomous mode controller
		/// This class contains the code to execute an automode program and
		/// the derived classs generally creates the auto program for a given
		/// robot, automode selector, and set of field conditions.
		class AutoController : public ControllerBase {
		public:
			AutoController(Robot &robot):ControllerBase(robot) {				
			}

			virtual void run() {
				if (actionptr_ != nullptr) {
					actionptr_ ->run();

					if (actionptr_->isDone())
						actionptr_ = nullptr ;
				}
			}
			
		protected:
			/// \brief sets the reference to the action that will be run when run() is called
			/// \param action run for a controller
			void setAction(ActionPtr action){
				actionptr_ = action;
				if (actionptr_ != nullptr)
					actionptr_->start() ;
			}

		private:
			ActionPtr actionptr_;
		} ;
	}
}
