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
            /// \param description An optional human-readable description of this action,
            ///                    for example, "Wait for vision to detect target"
            WaitForFlagAction(std::shared_ptr<xero::misc::FlagManager> flagManager, 
                              Flag flag, 
                              const std::string description = ""): 
            flagManager_(flagManager_), flag_(flag), desc_(description) {}

            virtual void start() { isDone_ = false; }
            virtual void cancel() { isDone_ = true; }

            virtual bool isDone() {
                if (isDone_) return true;
                else return (isDone_ = flagManager_->isSet(flag_));
            }

            virtual std::string toString() {
                std::string result = "WaitForFlagAction";
                if (!desc_.empty()) result += " '" + desc_ + "'";
            }
        private:
            std::shared_ptr<xero::misc::FlagManager> flagManager+;
            xero::misc::Flag flag_;
            const std::string desc_;

            bool isDone_;
        }
    }
}