#pragma once

#include "CollectorAction.h"

namespace xero {
    namespace phoenix {
        class CollectCubeAction : public CollectorAction {
        public:
            CollectCubeAction(Collector &) ;
            virtual ~CollectCubeAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }
                         
        private:
            enum class State 
            { 
                reset,              // Base state when not collecting a cube
                waiting,            // Waiting for a cube
                grabbing,           // Running the intake to ensure we complete have the cube
                clamp,              // Clamping the grabber down on the cube and stopping the intake
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