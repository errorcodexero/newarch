#pragma once 

#include "GamePieceAction.h"
#include "GamePieceManipulator.h"
#include "turntable/TurntableGoToAngleAction.h"

namespace xero{
    namespace phaser{
        class ReadyAction : public GamePieceAction {
        public:
            ReadyAction(GamePieceManipulator &subsystem, const std::string &height, const std::string &angle, bool leave = false) ;
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
                WaitForStop,
                ExtendHatchHolder,
                TurntableAndLift,
                RetractHatchHolder,
                TurntableAndSafeHeight,
                LifterSafeHeight,
                LifterFinalHeight,
                Idle,
            } ;

        private:
            std::string toString(State st) {
                std::string ret = "????" ;

                switch(st) {
                    case State::WaitForStop:
                        ret = "WaitForStop" ;
                        break ;
                    case State::ExtendHatchHolder:
                        ret = "ExtendHatchHolder" ;
                        break ;
                    case State::TurntableAndLift:
                        ret = "TurntableAndLift" ;
                        break ;                    
                    case State::RetractHatchHolder:
                        ret = "RetractHatchHolder" ;
                        break ;                    
                    case State::TurntableAndSafeHeight:
                        ret = "TurntableAndSafeHeight" ;
                        break ;                    
                    case State::LifterSafeHeight:
                        ret = "LifterSafeHeight" ;
                        break ;                    
                    case State::LifterFinalHeight:
                        ret = "LifterFinalHeight" ;
                        break ;                    
                    case State::Idle:
                        ret = "Idle" ;
                        break ;                    
                }

                return ret ;
            }

        private:
            State state_ ;
            State oldstate_ ;
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

            double turntable_velocity_threshold_ ;
            bool leave_ ;
        } ;
    }
}