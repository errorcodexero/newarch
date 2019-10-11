#pragma once

#include "CollectorAction.h"

namespace xero {
    namespace phoenix {
        class CollectTubAction : public CollectorAction {
        public:
            CollectTubAction(Collector &) ;
            virtual ~CollectTubAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone()  ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }
                         
        private:
            enum class State 
            { 
                reset,              // Base state when not collecting a Tub
                waiting,            // Waiting for a Tub
                grabbing,           // Running the intake to ensure we complete have the Tub
                clamp,              // Clamping the grabber down on the Tub and stopping the intake
                cancel
            } ;         

        private:
            std::string toString(State st) ;

        private:
            static std::string action_name ;


        private:
            double grab_time_ ;
            double start_ ;
            State state_ ;
            State prev_state_ ;
        } ;
    }
}