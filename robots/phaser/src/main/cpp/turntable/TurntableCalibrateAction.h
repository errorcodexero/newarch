#pragma once

#include "TurntableAction.h"
#include "Turntable.h"

namespace xero {
    namespace phaser {
        class TurntableCalibrateAction : public TurntableAction {
        public:
            TurntableCalibrateAction(Turntable &Turntable) ;
            virtual ~TurntableCalibrateAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual bool isDone() ;

            virtual void cancel() {
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            void getMinMax(int &minval, int &maxval) ;            

        private:
            static std::string action_name ;
            bool is_done_ ;
            std::list<int> counts_ ;
            int threshold_ ;
            int encbase_ ;
            int samples_ ;
            double power_ ;
        } ;
    }
}
