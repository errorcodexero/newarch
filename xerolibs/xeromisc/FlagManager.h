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
            Flag() : id_(nextID_++) {}

            bool operator<(const Flag rhs) const { return id_ < rhs.id_; }
        private:
            const int id_;
            static int nextID_;
        };

        /// FlagManager tracks the state of flags.
        /// \ref Flag
        class FlagManager {
        public:
            /// Sets the specified flag (creating it if it doesn't exist).
            void set(Flag flag) { update(flag, true); }

            /// Clears the specified flag (creating it if it doesn't exist).
            void reset(Flag flag) { update(flag, false); };

            /// Sets or clears the specified flag.
            /// \param flag The flag.
            /// \param value The value to assign to the flag.
            void update(Flag flag, bool value) { flags_[flag] = value; }

            /// \return true if the specified flag exists.
            /// A flag exists if it has been created by \c set or \c reset.
            bool exists(Flag flag) { return flags_.find(flag) != flags_.end(); };

            /// \return true if the specified flag exists and is set.
            bool isSet(Flag flag) { return exists(flag) && flags_.at(flag); };
        private:
            std::map<Flag, bool> flags_;
        };
    }
}