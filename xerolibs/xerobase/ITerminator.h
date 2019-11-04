#pragma once

namespace xero
{
    namespace base {
        class ITerminator {
        public:
            ITerminator(const std::string &name) {
                name_ = name ;
            }

            /// \brief returns true if the current action should be terminated
            /// \returns the termination state of the current action
            virtual bool shouldTerminate() = 0 ;

            const std::string &getTerminatorName() {
                return name_ ;
            }

        private:
            std::string name_ ;
        } ;
    }
}