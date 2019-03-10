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
                LifterUp,
                TurntableCalibrate,
                TurntableNorth,
                LifterCalibrate,
                Idle
            } ;

        private:
            xero::base::ActionPtr lifter_calibrate_ ;
            xero::base::ActionPtr turntable_north_ ;
            xero::base::ActionPtr turntable_calibrate_ ;
            xero::base::ActionPtr lifter_up_ ;
            xero::base::ActionPtr lifter_hold_ ;

            double lifter_up_duration_ ;
            double lifter_up_start_ ;
            State state_ ;
        } ;
    }
}