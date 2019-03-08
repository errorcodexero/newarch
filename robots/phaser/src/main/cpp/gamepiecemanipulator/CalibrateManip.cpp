#include "CalibrateManip.h"
#include <lifter/LifterCalibrateAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableCalibrateAction.h"
using namespace xero::base ;

namespace xero {
    namespace phaser {
        CalibrateManip::CalibrateManip(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {
            auto lifter = subsystem.getLifter() ;
            auto turntable = subsystem.getTurntable() ;

            lift_calibrate_ = std::make_shared<LifterCalibrateAction>(*lifter, 9.0, -0.1, 4) ;
            lift_to_safe_height = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            turntable_calibrate_ = std::make_shared<TurntableCalibrateAction>(*turntable) ;
        }

        CalibrateManip::~CalibrateManip(){
        }

        void CalibrateManip::start() {
            auto lifter = getGamePiece().getLifter() ;

            state_ = State::LifterCalibrate ;
            lifter->setAction(lift_calibrate_) ;
        }

        void CalibrateManip::run(){
            switch(state_) {
                case State::LifterCalibrate:
                    if (lift_calibrate_->isDone()) {
                        state_ = State::LifterToSafeHeight ;
                        auto lifter = getGamePiece().getLifter() ;
                        lifter->setAction(lift_to_safe_height) ;
                    }
                break ;

                case State::LifterToSafeHeight:
                    if (lift_to_safe_height->isDone()) {
                        state_ = State::TurntableCalibrate ;
                        auto turntable = getGamePiece().getTurntable() ;
                        turntable->setAction(turntable_calibrate_) ;
                    }
                break ;

                case State::TurntableCalibrate:
                    if (turntable_calibrate_->isDone()) {
                        state_ = State::Idle ;
                    }
                break ;

                case State::Idle:
                break ;
            }
        }

        bool CalibrateManip::isDone() {
            return state_ == State::Idle ;
        }

        void CalibrateManip::cancel() {
        }

        std::string CalibrateManip::toString() {
            std::string ret("CalibrateManip") ;
            return ret ;
        }
    }
}