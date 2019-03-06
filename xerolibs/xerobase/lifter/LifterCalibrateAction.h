#pragma once

#include "LifterAction.h"
#include "Lifter.h"
#include <list>

namespace xero {
    namespace base {
        class LifterCalibrateAction : public LifterAction {
        public:
            LifterCalibrateAction(Lifter &lifter, double height, double power, size_t samples) ;
            virtual ~LifterCalibrateAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual bool isDone() {
                return is_done_ ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() ;

        private:
            double power_ ;
            double height_ ;
            bool is_done_ ;
            size_t samples_ ;
            std::list<int> counts_ ;
        } ;
    }
}
