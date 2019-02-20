#include "ResetIntakesAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterHoldHeightAction.h>
#include <cargointake/CargoIntake.h>
#include <cargointake/CargoIntakeAction.h>
#include <hatchintake/HatchIntake.h>
#include <hatchintake/HatchIntakeAction.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ResetIntakeAction::ResetIntakeAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {

            auto lifter = getGamePiece().getLifter() ; 
            auto hatch_intake = getGamePiece().getHatchIntake();
            auto cargo_intake = getGamePiece().getCargoIntake() ;

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            hold_lifter_safe_height_  = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            retract_hatch_intake_ = std::make_shared<HatchIntakeAction>(*hatch_intake, false) ;
        }

        ResetIntakeAction::~ResetIntakeAction() {
        }

        void ResetIntakeAction::start() {
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_intake = getGamePiece().getHatchIntake();
            auto cargo_intake = getGamePiece().getCargoIntake() ;

            if (!hatch_intake->isDeployed() && !cargo_intake->isDeployed())
                state_ = State::Idle ;
            else 
            {
                lifter->setAction(set_lifter_safe_height_) ;
                state_ = State::Lifting ;
            }
        }

        void ResetIntakeAction::run() {
            switch(state_) {
            case State::Lifting:
                if (set_lifter_safe_height_->isDone()) {
                    auto hatch_intake = getGamePiece().getHatchIntake();
                    hatch_intake->setAction(retract_hatch_intake_) ;

                    auto cargo_intake = getGamePiece().getCargoIntake();  
                    cargo_intake->setAction(retract_cargo_intake_) ;

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(hold_lifter_safe_height_) ;
                }
                break ;

            case State::Retracting:
                if (retract_hatch_intake_->isDone() && retract_cargo_intake_->isDone()) {
                    state_ = State::Idle ;
                }
                break ;
            }
        }
        
        bool ResetIntakeAction::isDone() {
            return state_ == State::Idle ;
        }

        void ResetIntakeAction::cancel() {
        }

        std::string ResetIntakeAction::toString() {
            std::string ret("ResetIntakeAction") ;
            return ret ;
        }
    }
}