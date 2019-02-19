#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"

namespace xero{
    namespace phaser{
        class ReadyAction : public GamePieceAction {
        public:
            ReadyAction(GamePieceManipulator &subsystem, const std::string &height, const std::string &angle) ;
            virtual ~ReadyAction() ;


            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State{
                Idle,
                LifterSafeHeight,
                TurntableGoToAngle,
                LifterGoToFinalHeight,
            } ;

        private:
            State state_ ;
            std::string height_ ;
            std::string angle_ ;

            xero::base::ActionPtr set_lifter_safe_height_ ;
            xero::base::ActionPtr hold_lifter_safe_height_ ;
            xero::base::ActionPtr set_lifter_final_height_ ;
            xero::base::ActionPtr hold_lifter_final_height_ ;
            xero::base::ActionPtr set_turntable_angle_ ;
            xero::base::ActionPtr hold_turntable_angle_ ;                
        } ;
    }
}