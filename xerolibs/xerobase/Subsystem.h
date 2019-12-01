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
            /// \brief A specific subsystem that does not have a parent
            /// RobotSubsystem doesn't have a parent, and therefore needs to use a private constructor
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

            /// \brief the return status from a setAction call
            enum class SetActionResult {
                Accepted,           ///< the action was accepted
                PreviousCanceled,   ///< The action was accepted, but replaced a previous action (either on this subsystem or on a child subsystem).
                Rejected,           ///< The action was rejected because canAcceptAction returned false.
                ParentBusy          ///< The action was rejected because the parent subsystem is busy.
            };

            /// \brief Returns true if the result means the action was accepted
            /// \param r the result to test
            /// \returns true if the action was accepted given the result
            static inline bool isAccepted(SetActionResult r) { 
                return r == SetActionResult::Accepted || r == SetActionResult::PreviousCanceled;
            }

            /// \brief set the current Action for the subsystem
            /// \param action the new Action for the subsystem
            /// \param allowParentBusy \c allowParentBusy is false, this method will fail with result \c ParentBusy
            /// \return A \c SetActionResult describing the result of the operation.
            virtual SetActionResult setAction(ActionPtr action, bool allowParentBusy = false);

            /// \brief Returns this subsystem's default action
            /// The default action runs whenever no other actions are running.
            /// \returns the default action for this subsystem, or nullptr if one has not been assigned
            ActionPtr getDefaultAction() const { return defaultAction_; }

            /// \brief Sets this subsystem's default action
            /// \param action the action to set as the default action
            /// \return true if the action was accepted
            bool setDefaultAction(ActionPtr action);

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

            /// \brief Initialize a plot session for a subsystem
            /// and return a handle to the plot session.  This handle must be used
            /// an all other plot related methods.
            /// \note Multiple plot sessions, even with the same name can be open concurrently
            /// \param name the name of the plot session
            /// \returns a handle to an open plot session
            int initPlot(const std::string &name)  ;

            /// \brief start a plot session. 
            /// If a previous plot session exists, the data is removed in preparation for a new
            /// plot session.  The data is stored in the network tables in a specific format.  See
            /// the software wiki for more information on this data format.
            /// \param id the handle from an initPlot() call
            /// \param cols an array of column names for the data that will be provided in the plot
            /// \sa initPlot            
            void startPlot(int id, const std::vector<std::string> &cols) ;

            /// \brief add a row of data to the plot
            /// The vector of values must be of the same size as the vector of columns in the
            /// start plot call.
            /// \param id the handle from an initPlot() call
            /// \param values the set of values for this row of plot data
            /// \sa initPlot            
            void addPlotData(int id, const std::vector<double> &values) ;

            /// \brief end the current plot sesssion signaling all data is present
            /// This does NOT deinitialze the plot session.  It on informs any client
            /// watching the data that no more data will be pfovided.
            /// \param id the handle from an initPlot() call
            /// \sa initPlot
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

            /// \brief Checks that an is valid as a subsystem's default action.
            /// It is expected that a derived class will override this method to 
            /// accept actions as default actions.
            /// \param action the action to test
            /// \returns true if the action can be the default action, otherwise false
            virtual bool canAcceptDefaultAction(ActionPtr action) {
                return false;
            }

        private:
            Subsystem(Robot &robot, const std::string &name);

            bool _canAcceptAction(ActionPtr action, bool isDefault = false);

            bool parentBusy();

            // Cancels actions on this subsystem and all its descendants.
            // Returns true if any actions were cancelled.
            bool cancelActionsAndChildActions(std::shared_ptr<Action> action);
            
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
            // The default action
            //
            ActionPtr defaultAction_;

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
