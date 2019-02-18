#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ScoreHatch : public GamePieceAction{
            ScoreHatch(GamePieceManipulator &subsystem) ;
            virtual ~ScoreHatch() ;


            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

            private:
                enum class State{
                    Idle,
                    TurntableGoToAngle,
                    LifterGoToScoreHeight,
                    ExtendArm,
                    ReleaseFinger,
                    LifterShiftDownHeight,
                    RetractArm,
                    CheckHolder,
                } ;

            private:
                State state_ ;

                xero::base::ActionPtr set_lifter_score_height_ ;
                xero::base::ActionPtr hold_lifter_score_height_ ;
                xero::base::ActionPtr set_turntable_cargo_angle_ ;
                xero::base::ActionPtr set_lifter_shift_down_height_ ;
                
                xero::base::ActionPtr set_extend_arm_ ;
                xero::base::ActionPtr set_retract_arm_ ;
                xero::base::ActionPtr set_retract_hatch_finger_ ;
                xero::base::ActionPtr set_deploy_hatch_finger_ ;
        } ;
    }
}