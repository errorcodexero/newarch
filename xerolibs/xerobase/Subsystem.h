#pragma once

#include "actions/Action.h"
#include "LoopType.h"
#include <memory>
#include <map>
#include <list>
#include <string>
#include <vector>

/// \file


namespace xero {
    namespace base {
        //
        // Forward declaration of the robot class.
        //
        class Robot ;
        
        class Subsystem;
        typedef std::shared_ptr<Subsystem> SubsystemPtr ;

        /// \brief The base class for any subsystem in the system.
        class Subsystem {
             // RobotSubsystem doesn't have a parent, and therefore needs to use a private constructor
            friend class RobotSubsystem;
        public: 
            /// \brief create a new subsystem
            /// \param robot the robot
            /// \param name the name of the subsystem
            Subsystem(Subsystem *parent, const std::string &name) ;

            /// \brief destroy a new subsystem
            virtual ~Subsystem() ;

            /// \brief Returns the name of the subsystem
            /// \returns The name of the subsystem
            const std::string &getName() const {
                return name_ ;
            }

            /// \brief add a subsystem as a child of the current subsystem
            /// \param child the subsystem to add as a child to the current subsystem
            void addChild(std::shared_ptr<Subsystem> child) {
                children_.push_back(child) ;
            }

            /// \brief compute the current state of the robot.
            /// The subsystem generally reads and associated input sensors and
            /// computes a state that is meaningful to users of the subsystem.
            virtual void computeState() ;


            enum class SetActionResult {
                // The action was accepted.
                Accepted,

                // The action was accepted, but replaced a previous action.
                PreviousCanceled,

                // The action was rejected because canAcceptAction returned false.
                Rejected,  

                // The action was rejected because the parent subsystem is busy.
                ParentBusy
            };

            static inline bool isAccepted(SetActionResult r) { 
                return r == SetActionResult::Accepted || r == SetActionResult::PreviousCanceled;
            }

            /// \brief set the current Action for the subsystem
            /// \param action the new Action for the subsystem
            /// \param isParent Whether the new action is being set by a parent subsystems. If a parent
            /// subsystem is busy and \c isParent is false, this method will fail with result \c parentBusy
            /// \return true if the Action is accepted, false if not
            virtual SetActionResult setAction(ActionPtr action, bool isParent = false);

            /// \brief Returns this subsystem's default action
            /// The default action runs whenever no other actions are running.
            virtual ActionPtr getDefaultAction() { return nullptr; }

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

            /// \returns A pointer to the parent subsystem
            Subsystem *getParent() { 
                return parent_;
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

            /// \brief Tests this subsystem
            virtual void selfTest() {}
            
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

            /// \brief this is called after hardware is initialized.
            /// This method gives each subsystem a chance to initialization
            /// any hardware needed.
            /// 
            virtual void postHWInit() {
                for(auto child : children_)
                    child->postHWInit() ;
            }
            
            int initPlot(const std::string &name)  ;

            void startPlot(int id, const std::vector<std::string> &cols) ;

            void addPlotData(int id, const std::vector<double> &values) ;

            void endPlot(int id) ;

            /// \return true if the subsystem is currently executing an action
            bool isBusy();

            /// \return true if the subsystem or one if its ancestors is currently executing an action
            bool isBusyOrParentBusy();

            /// \return true if the subsystem or one if its children is currently executing an action
            bool isBusyOrChildBusy();

        protected:
            /// \brief check that a Action is valid for a subsystem
            /// \param Action the Action to check for a subsystem
            /// \return true if the action is valid for a subsystem
            virtual bool canAcceptAction(ActionPtr action) {
                return false ;
            }

        private:
            Subsystem(Robot &robot, const std::string &name);

            bool _canAcceptAction(ActionPtr action);

            bool parentBusy();
            
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

            // Whether the currently active action is the default action.
            bool isRunningDefaultAction_;

            // The parent subsystem
            Subsystem *parent_;

            //
            // The set of child subsystems
            //
            std::list<std::shared_ptr<Subsystem>> children_ ;
        } ;
    }
}
