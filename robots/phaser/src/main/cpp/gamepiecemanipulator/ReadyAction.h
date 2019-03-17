#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"
#include "turntable/TurntableGoToAngleAction.h"

namespace xero{
    namespace phaser{
        class ReadyAction : public GamePieceAction {
        public:
            ReadyAction(GamePieceManipulator &subsystem, const std::string &height, const std::string &angle) ;
            ReadyAction(GamePieceManipulator &subsystem, double height, const std::string &angle) ;            
            virtual ~ReadyAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            bool alreadyOnCorrectSide() ;
            void startTurnLiftSequence() ;

        private:
            enum class State {
                ExtendHatchHolder,
                TurntableAndLift,
                RetractHatchHolder,
                TurntableAndSafeHeight,
                LifterSafeHeight,
                LifterFinalHeight,
                Idle,
            } ;

        private:
            State state_ ;
            std::string height_ ;
            std::string angle_ ;

            double height_value_ ;
            double angle_value_ ;


            bool hatch_holder_extended_ ;

            xero::base::ActionPtr set_lifter_safe_height_ ;
            xero::base::ActionPtr set_lifter_final_height_ ;
            xero::base::ActionPtr extend_hatch_holder_ ;
            xero::base::ActionPtr retract_hatch_holder_ ;            
            std::shared_ptr<TurntableGoToAngleAction> set_turntable_angle_ ;
        } ;
    }
}