#pragma once

#include "HIDDevice.h"
#include "Action.h"

namespace xero {
    namespace base {
        class TankDrive ;

        class DriverGamepad : public HIDDevice {
        public:
            enum AxisNumber : int {
                LEFTX = 0,
                LEFTY = 1, 
                LTRIGGER = 2,
                RTRIGGER = 3,
                RIGHTX = 4,
                RIGHTY = 5
            } ;

            enum ButtonNumber : int {
                A = 0,
                B = 1,
                X = 2,
                Y = 3,
                LB = 4,
                RB = 5,
                BACK = 6,
                START = 7,
                L_JOY = 8,
                R_JOY = 9
            } ;

            enum POVAngle : int {
                UP = 0,
                UPRIGHT = 45,
                RIGHT = 90,
                DOWNRIGHT = 135,
                DOWN=180,
                DOWNLEFT = 225,
                LEFT = 270,
                UPLEFT = 315, 
            } ;

        public:
            DriverGamepad(Subsystem &oi, int index) ;
            virtual ~DriverGamepad() ;

            virtual void computeState(ActionSequence &) ;

			void init(std::shared_ptr<TankDrive> db) ;

        private:
            double scalePower(double axis, double boost, bool slow) ;
            
        private:
            static const int SpinAxis = AxisNumber::RIGHTX ;
            static const int TurboAxis = AxisNumber::LTRIGGER ;

            static const int SlowButton = ButtonNumber::LB ;

        private:
            std::shared_ptr<TankDrive> db_ ;
            int pov_ ;
            double default_duty_cycle_ ;
            double max_duty_cycle_ ;
            double slow_factor_ ;

            double left_ ;
            double right_ ; 
            double tolerance_ ;

            ActionPtr nudge_forward_ ;
            ActionPtr nudge_backward_ ;
            ActionPtr nudge_clockwise_ ;
            ActionPtr nudge_counter_clockwise_ ;
        } ;
     }
 }