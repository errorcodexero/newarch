#pragma once

#include <map>
#include <string>

namespace xero {
    namespace misc {
        /// A Flag allows Boolean state information to be communicated across subsystems.
        /// To create a flag, declare it as a static constant:
        ///     static const Flag myFlag;
        /// To use a flag, pass it to a FlagManager:
        ///     if (robot.getFlagManager().isSet(SomeSubystem::myFlag)) { /* flag is set */ }
        /// \note To ensure deterministic behavior, subsystems should (by convention)
        ///    update flags in \c computeState and check them in \c run.
        ///    By convention, the subsystem that owns a flag should be responsible
        ///    for setting and clearing it.
        /// \ref FlagManager
        class Flag {
        public:
            /// Creates a unique Flag.
            /// @param description The flag name (only for debugging/logging).
            Flag(std::string description) : id_(nextID_++), description_(description) {}

            bool operator<(const Flag rhs) const { return id_ < rhs.id_; }
            bool operator==(const Flag rhs) const { return id_ == rhs.id_; }

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
            FlagManager(MessageLogger &logger, uint64_t msggroup): logger_(logger), msggroup_(msggroup) {}

            /// Sets the specified flag (creating it if it doesn't exist).
            void set(Flag flag) { update(flag, true); }

            /// Clears the specified flag (creating it if it doesn't exist).
            void reset(Flag flag) { update(flag, false); };

            /// Sets or clears the specified flag.
            /// \param flag The flag.
            /// \param value The value to assign to the flag.
            void update(Flag flag, bool value);

            /// \return true if the specified flag exists.
            /// A flag exists if it has been created by \c set or \c reset.
            bool exists(Flag flag) { return flags_.find(flag) != flags_.end(); };

            /// \return true if the specified flag exists and is set.
            bool isSet(Flag flag) { return exists(flag) && flags_.at(flag); };
        private:
            std::map<Flag, bool> flags_;
            MessageLogger &logger_;
            uint64_t msggroup_;
        };
    }
}