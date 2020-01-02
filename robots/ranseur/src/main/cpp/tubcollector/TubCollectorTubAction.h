#pragma once

#include "TubCollectorAction.h"
#include <string>
#include <vector>

namespace xero {
    namespace ranseur {
        class TubCollectorTubAction : public TubCollectorAction {
        public:
            TubCollectorTubAction(TubCollector &) ;
            virtual ~TubCollectorTubAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }
                         
        private:
            enum class State 
            { 
                motors_on,                    //motors on and ready for intake
                closed_hand,                   //close the hand with solenoid
                done,                         //done collecting tub (just turn motors on barely to hold tubs?)
            } ;         

        private:
            std::string toString(State st) ;

        private:
            static std::string action_name ;

        private:
            double grab_time_ ;
            double start_ ;
            State state_ ;
            double speed_ ;
            double delay_ ;

            int plotid_ ;
            static std::vector<std::string> cols_ ;
        } ;
    }
}
