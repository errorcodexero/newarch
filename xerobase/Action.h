#pragma once
#include <memory>
#include <vector>
#include <Timer.h>
#include <string>

namespace xero {
    namespace base {
        class Action {
        public:
            virtual void start() = 0 ;
            virtual void run() = 0 ;
            virtual bool isDone() = 0 ;
            virtual void cancel() = 0 ;
            virtual std::string toString() = 0 ;
        private:
        };

        typedef std::shared_ptr<Action> ActionPtr ;
    }
}
