#include "TurntableCalibrateAction.h"

namespace xero {
    namespace phaser {

        std::string TurntableCalibrateAction::action_name("TurntableCalibrate") ;

        TurntableCalibrateAction::TurntableCalibrateAction(Turntable &Turntable) : TurntableAction(Turntable) {            
        }

        TurntableCalibrateAction::~TurntableCalibrateAction() {            
        }

        void TurntableCalibrateAction::start() {
            getTurntable().setMotorPower(-0.1) ;
            is_done_ = false ;
        }

        void TurntableCalibrateAction::run() {
            ticks_.push_back(getTurntable().getEncoderValue()) ;
            if (ticks_.size() > 4)
                ticks_.pop_front() ;
        }

        bool TurntableCalibrateAction::isDone() {
            return true ;
        }
    }
}