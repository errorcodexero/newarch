#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class CalibrateManip : public GamePieceAction {
        public:
            CalibrateManip(GamePieceManipulator &subsystem) ;
            virtual ~CalibrateManip() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State {
                LifterCalibrate,
                LifterToSafeHeight,
                TurntableCalibrate,
                Idle
            } ;

        private:
            xero::base::ActionPtr lift_calibrate_ ;
            xero::base::ActionPtr lift_to_safe_height ;
            xero::base::ActionPtr turntable_calibrate_ ;

            State state_ ;
        } ;
    }
}