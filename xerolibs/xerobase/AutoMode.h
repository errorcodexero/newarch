#pragma once

#include "actions/SequenceAction.h"
#include "Robot.h"


namespace xero {
    namespace base {
        class AutoMode : public SequenceAction {

        public:
            /// \brief create an empty action sequence 
            /// \param logger the message logger action
            /// \param name the human readable name of this sequence
            /// \param desc a description of this automode
            AutoMode(Robot &robot, const std::string &name, const std::string &desc) ;
            
            virtual ~AutoMode() ;

            /// \brief return the name of the action sequence
            /// \returns the name of the action sequence
            const std::string &getName() const {
                return name_ ;
            }

            const std::string &getDesc() const {
                return desc_ ;
            }

            /// \brief create a human readable string representing this action sequence
            /// \returns a human readable string representing this action sequence
            std::string toString();

        protected:
            Robot &getRobot() {
                return robot_ ;
            }

        private:

            // The robot object
            Robot &robot_ ;

            // names the overall objective of the actions in the sequence
            std::string name_;

            // human readable description of the auto mode
            std::string desc_ ;
        };

        
        typedef std::shared_ptr<AutoMode> AutoModePtr ;

    }
}