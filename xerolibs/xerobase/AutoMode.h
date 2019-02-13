#pragma once

#include "ActionSequence.h"



namespace xero {
    namespace base {
        class AutoMode : public ActionSequence {

            public:
            /// \brief create an empty action sequence 
            /// \param logger the message logger action
            /// \param name the human readable name of this sequence
            /// \param desc a description of this automode
            AutoMode(xero::misc::MessageLogger &logger, const std::string &name, const std::string &desc) ;

            /// \brief create an empty action sequence 
            /// \param logger the message logger action
            /// \param name the human readable name of this sequence
            AutoMode(xero::misc::MessageLogger &logger, const std::string &name) ;

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


            private:
            //names the overall objective of the actions in the sequence
            std::string name_;

            std::string desc_ ;


        };

        
        typedef std::shared_ptr<AutoMode> AutoModePtr ;

    }
}