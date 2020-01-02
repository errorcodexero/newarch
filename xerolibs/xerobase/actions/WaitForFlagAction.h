#pragma once

#include <memory>

#include "Action.h"
#include <FlagManager.h>

namespace xero {
    namespace base {
        /// An Action which waits for a flag to be set.
        class WaitForFlagAction: public GenericAction {
        public:
            /// Constructs a WaitForFlagAction.
            /// \param flagManager The flag manager.
            /// \param flag The flag to wait for.
            WaitForFlagAction(xero::misc::MessageLogger &logger, 
                              std::shared_ptr<xero::misc::FlagManager> flagManager, 
                              xero::misc::Flag flag): 
                GenericAction(logger), flagManager_(flagManager_), flag_(flag) 
            {

            }

            virtual void start() {
            }

            virtual void cancel() {
                setDone();
            }

            virtual void run() {
                if (flagManager_->isSet(flag_))
                    setDone();
            }

            virtual std::string toString() {
                std::string result = "WaitForFlagAction";
                result += " '" + flag_.getDescription() + "'";
                return result;
            }
        private:
            std::shared_ptr<xero::misc::FlagManager> flagManager_;
            xero::misc::Flag flag_;
        };
    }
}