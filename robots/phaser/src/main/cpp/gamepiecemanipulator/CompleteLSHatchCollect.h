# pragma once

# include "GamePieceManipulator.h"
# include "GamePieceAction.h"

namespace xero {
    namespace phaser {
        class CompleteLSHatchCollect : public GamePieceAction {
        public:
            CompleteLSHatchCollect(GamePieceManipulator &subsystem) ;
            virtual ~CompleteLSHatchCollect() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State {
                Idle,
                RetractFinger,
                ExtendArm,
                LifterShiftUpHeight,
                DeployFinger,
                RetractArm,
        } ;

        private:
            State state_ ;
            
            xero::base::ActionPtr set_lifter_shift_up_height_ ;
            xero::base::ActionPtr set_extend_arm_ ;
            xero::base::ActionPtr set_retract_arm_ ;
            xero::base::ActionPtr set_retract_hatch_finger_ ;
            xero::base::ActionPtr set_deploy_hatch_finger_ ;
        } ;
    }
}