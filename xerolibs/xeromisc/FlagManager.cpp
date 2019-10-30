#include "FlagManager.h"
#include "MessageLogger.h"

namespace xero {
    namespace misc {
        void FlagManager::update(Flag flag, bool value) { 
                logger_.startMessage(MessageLogger::MessageType::debug, msggroup_);

                auto it = flags_.find(flag);
                if (it == flags_.end()) {
                    // The flag did not previously exist.
                    logger_ << "FlagManager: Created and " << (value ? "set" : "cleared");
                    logger_ << " flag '" << flag.getName() << "'";

                    flags_.insert(std::make_pair(flag, value));
                } else {
                    // The flag already existed
                    logger_ << "FlagManager: " << (value ? "Set" : "Cleared") << " flag '" << flag.getName();
                    logger_ << "' (was previously " << (it->second ? "set" : "cleared") << ")";

                    it->second = value;
                }

                logger_.endMessage();
            }
    }
}