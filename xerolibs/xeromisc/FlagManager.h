#pragma once

#include <map>
#include <string>

namespace xero {
    namespace misc {
        /// @brief A Flag allows Boolean state information to be communicated across subsystems.
        /// To create a flag, declare it as a static constant:
        ///     static const Flag myFlag;
        /// To use a flag, pass it to a FlagManager:
        ///     if (robot.getFlagManager().isSet(SomeSubystem::myFlag)) { /* flag is set */ }
        /// @note To ensure deterministic behavior, subsystems should (by convention)
        ///    update flags in \c computeState and check them in \c run.
        ///    By convention, the subsystem that owns a flag should be responsible
        ///    for setting and clearing it.
        /// @ref FlagManager
        class Flag {
        public:
            /// Creates a unique Flag.
            /// @param description The flag name (only for debugging/logging).
            Flag(std::string description) : id_(nextID_++), description_(description) {}

            /// @brief returns true if the flag is less than the other flag given
            /// @param rhs the other flag to compare to
            /// @returns true if the flag is less than the other flag given
            bool operator<(const Flag rhs) const { return id_ < rhs.id_; }

            /// @brief returns true if the flag is the same as the other flag given
            /// @param rhs the other flag to compare to            
            /// @return true if the flag is the same as the other flag given
            bool operator==(const Flag rhs) const { return id_ == rhs.id_; }

            /// @brief return the description of the flag
            /// @returns the description of the flag
            std::string getDescription() { return description_ + " (" + std::to_string(id_) + ")"; }

        private:
            int id_;
            std::string description_;

            static int nextID_;
        };

        class MessageLogger;

        /// FlagManager tracks the state of flags.
        /// \ref Flag
        class FlagManager {
        public:
            /// @brief createa a new flag manager
            /// @param logger the message logger to use to send messages
            /// @param msggroup the message group number to use for the message logger
            FlagManager(MessageLogger &logger, uint64_t msggroup): logger_(logger), msggroup_(msggroup) {}

            /// @brief Sets the specified flag (creating it if it doesn't exist).
            /// @param flag the flag to set
            void set(Flag flag) { update(flag, true); }

            /// @brief Clears the specified flag (creating it if it doesn't exist).
            /// @param flag the falg to clear
            void reset(Flag flag) { update(flag, false); };

            /// Sets or clears the specified flag.
            /// @param flag The flag to set or clear
            /// @param value The value to assign to the flag.
            void update(Flag flag, bool value);

            /// @brief returns true if a flag exists
            /// A flag exists if it has been created by \c set or \c reset.
            /// @return true if the specified flag exists.
            /// @param flag the flag to test for existance
            bool exists(Flag flag) { return flags_.find(flag) != flags_.end(); };

            /// @brief returns true if the flag is set
            /// @return true if the specified flag exists and is set.
            /// @param flag the flag to test its value
            bool isSet(Flag flag) { return exists(flag) && flags_.at(flag); };

        private:
            std::map<Flag, bool> flags_;
            MessageLogger &logger_;
            uint64_t msggroup_;
        };
    }
}