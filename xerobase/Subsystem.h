#pragma once

#include "Action.h"
#include "LoopType.h"
#include <memory>
#include <map>
#include <list>
#include <string>

/// \file


namespace xero {
	namespace base {
		//
		// Forward declaration of the robot class.
		//
		class Robot ;
		class Subsystem ;

        /// \brief convience definition
		typedef std::shared_ptr<Subsystem> SubsystemPtr ;
		
		/// \brief The base class for any subsystem in the system.
		class Subsystem {
		public:	
			/// \brief create a new subsystem
			/// \param robot the robot
			/// \param name the name of the subsystem
			Subsystem(Robot &robot, const std::string &name) ;

			/// \brief destroy a new subsystem
			virtual ~Subsystem() ;

			/// \brief Returns the name of the subsystem
			/// \returns The name of the subsystem
			const std::string &getName() const {
				return name_ ;
			}

			/// \brief add a subsystem as a child of the current subsystem
			/// \param child the subsystem to add as a child to the current subsystem
			void addChild(SubsystemPtr child) {
				children_.push_back(child) ;
			}

			/// \brief compute the current state of the robot.
			/// The subsystem generally reads and associated input sensors and
			/// computes a state that is meaningful to users of the subsystem.
     		virtual void computeState() ;

			/// \brief set the current Action for the subsystem
			/// \param Action the new Action for the subsystem
			/// \return true if the Action is accepted, false if not
			virtual bool setAction(ActionPtr Action);

			/// \brief return a constant pointer to the current Action
			/// \returns  a constant pointer to the current Action
			const ActionPtr getAction() const {
				return action_ ;
			}

			/// \brief returns a pointer to the current Action
			/// \returns a pointer to the current Action
			ActionPtr getAction()  {
				return action_ ;
			}

			/// \brief cancel the current action for this subsystem
			/// It also cancels the actions for any children subsystems
			/// \returns true if the action was canceled, false if it could not be
			virtual void cancelAction() ;

			/// \brief Set output actuators associated with the subsystem
			/// The output actuators are set to achieve the currently active
			/// Action based on the current state of the subsystem.
			virtual void run() ;

			/// \brief initialize the subsystem
			/// \param ltype the type of loop we are initializing for
			/// This is called after all of the subsystems are created.
			virtual void init(LoopType ltype) ;

			/// \brief reset a subsystem by remove all actions on the subsystem and its children
			virtual void reset() ;					
			
			/// \brief returns true if the subsystem is done with the current Action
			/// \returns true if the subsystem is done with the current Action
			virtual bool isDone() const {
				if (action_ == nullptr)
					return true ;
					
				return action_->isDone() ;
			}

			/// \brief return the main robot object
			/// \returns the main robot object
			Robot &getRobot() {
				return robot_ ;
			}


		protected:
			/// \brief check that a Action is valid for a subsystem
			/// \param Action the Action to check for a subsystem
			/// \return true if the action is valid for a subsystem
			virtual bool canAcceptAction(ActionPtr Action) {
				return false ;
			}

			/// \brief define a named sequence of actions
			/// 
			void defineNamedSequence(const char *name_p, ActionPtr action) {
				named_sequences_[name_p] = action ;
			}


		private:
			//
			// A reference to the robot object that contains this subsystem
			//
			Robot& robot_;

			//
			// The name of the subsystem (e.g. TankDrive)
			//
			std::string name_ ;

			//
			// The currently active Action
			//
			ActionPtr action_;

			//
			// The set of child subsystems
			//
			std::list<SubsystemPtr> children_ ;

			//
			// The set of named sequences
			//
			std::map<std::string, ActionPtr> named_sequences_ ;
		} ;

		typedef std::shared_ptr<Subsystem> SubsystemPtr ;
	}
}
