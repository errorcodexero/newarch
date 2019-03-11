#include "CalibrateManip.h"
#include "turntable/TurntableGoToAngleAction.h"
#include "turntable/TurntableCalibrateAction.h"
#include <lifter/LifterPowerAction.h>
#include <lifter/LifterCalibrateAction.h>
#include <iostream>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CalibrateManip::CalibrateManip(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {
            auto lifter = subsystem.getLifter() ;
            auto turntable = subsystem.getTurntable() ;

            lifter_calibrate_ = std::make_shared<LifterCalibrateAction>(*lifter) ;
            turntable_north_ = std::make_shared<TurntableGoToAngleAction>(*turntable, 0.0) ;
            turntable_calibrate_ = std::make_shared<TurntableCalibrateAction>(*turntable) ;
            lifter_up_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:calibrate:up:power") ;
            lifter_up_duration_ = subsystem.getRobot().getSettingsParser().getDouble("lifter:calibrate:up:time") ;
            lifter_hold_ = std::make_shared<LifterPowerAction>(*lifter, "lifter:calibrate:hold:power") ;
        }

        CalibrateManip::~CalibrateManip(){
        }

        void CalibrateManip::start() {
            auto lifter = getGamePiece().getLifter() ;

            state_ = State::LifterUp ;
            lifter->setAction(lifter_up_) ;
            lifter_up_start_ = getGamePiece().getRobot().getTime() ;
        }

        void CalibrateManip::run() {
            auto turntable = getGamePiece().getTurntable() ;            
            auto lifter = getGamePiece().getLifter() ;

            switch(state_) {
                case State::LifterUp:
                    if (getGamePiece().getRobot().getTime() - lifter_up_start_ > lifter_up_duration_) {
                        std::cout << "LifterUp done" << std::endl ;
                        state_ = State::TurntableCalibrate ;
                        turntable->setAction(turntable_calibrate_) ;
                        lifter->setAction(lifter_hold_) ;
                    }
                    break ;

                case State::TurntableCalibrate:
                    if (turntable_calibrate_->isDone()) {
                        std::cout << "TurntableCalibrate done" << std::endl ;                        
                        state_ = State::TurntableNorth;
                        turntable->setAction(turntable_north_) ;
                    }
                    break ;

                case State::TurntableNorth:
                    if (turntable_north_->isDone()) {
                        std::cout << "TurntableNorth done" << std::endl ;                          
                        state_ = State::LifterCalibrate ;
                        lifter->setAction(lifter_calibrate_) ;
                    }
                    break;

                case State::LifterCalibrate:
                    if (lifter_calibrate_->isDone()) {
                        std::cout << "LifterCalibrate done" << std::endl ;                            
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