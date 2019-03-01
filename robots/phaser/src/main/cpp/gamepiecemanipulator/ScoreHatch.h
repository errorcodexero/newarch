#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ScoreHatch : public GamePieceAction {
        public:
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
                ExtendArm,                    
                ReleaseFinger,
                LifterShiftDownHeight,
                RetractArm,
            } ;

        private:
            State state_ ;

            xero::base::ActionPtr set_lifter_shift_down_height_ ;
            xero::base::ActionPtr set_extend_arm_ ;
            xero::base::ActionPtr set_retract_arm_ ;
            xero::base::ActionPtr set_retract_hatch_finger_ ;
            xero::base::ActionPtr set_deploy_hatch_finger_ ;
            xero::base::ActionPtr rumble_ ;                
        } ;
    }
}