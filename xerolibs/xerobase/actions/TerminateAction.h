#include "Action.h"
#include "ITerminator.h"
#include "MessageLogger.h"
#include <list>
#include "Robot.h"
#include <memory>

namespace xero{
    namespace base {
        /// @brief This action runs a child action until a terminate condition is met
        /// The termination condition is met when any of the attached terminators returns
        /// true when the method shouldTerminate() is called.
        class TerminateAction : public CompositeAction {
        public:
            /// @brief Create a new TerminateAction object
            /// @param a the child action to run
            /// @param robot the robot object
            /// @param delay the amount of time to wait after this action runs before evaluating the terminate condition
            TerminateAction(ActionPtr a, Robot &robot, double delay = 0.0) ;

            /// @brief Create a new TerminateAction object
            /// @param sub the subsystem for the child action
            /// @param a the child action to run
            /// @param robot the robot object
            /// @param delay the amount of time to wait after this action runs before evaluating the terminate condition            
            TerminateAction(std::shared_ptr<Subsystem> sub, ActionPtr a, Robot &robot, double delay = 0.0) ;

            /// @brief start the termiante action
            virtual void start() ;

            /// @brief run the terminate action
            /// Evaluates the termination conditions.  If none return true to the shouldTerminate() call, then calls run
            /// on the child action.
            virtual void run() ;

            /// @brief called when this action is to be canceled.  
            /// Calls cancel on the child action
            virtual void cancel() ;

            /// @brief returns a string representation of the object
            /// @returns a string representation of the object
            virtual std::string toString() ;

            /// @brief add a terminator to the terminate action
            /// @param term a class derived from ITerminator that acts as a terminator for this child action
            void addTerminator(std::shared_ptr<ITerminator> term) {
                terminators_.push_back(term) ;
            }

            /// @brief returns all children of this action, i.e. the single child action
            /// @returns a vector containing the single child action.
            virtual std::vector<ActionPtr> getChildren() { return { action_ }; }

        private: 
            ActionPtr action_ ;
            std::list<std::shared_ptr<ITerminator>> terminators_ ;
            Robot &robot_ ;
            double delay_ ;
            double start_ ;
        };
    }
}