#pragma once

namespace xero {
    namespace base {
        class Action {
        public:
            virtual void start() = 0 ;
            virtual void end() = 0 ;
            virtual void run() = 0 ;
            virtual bool isDone() = 0 ;
            virtual bool cancel() = 0 ;
        private:
        } ;
    }
}