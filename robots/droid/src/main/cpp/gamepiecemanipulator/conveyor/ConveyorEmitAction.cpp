#include "ConveyorEmitAction.h"
#include "Conveyor.h"
#include <Robot.h>

using namespace xero::misc;

namespace xero {
    namespace droid {
        ConveyorEmitAction::ConveyorEmitAction(Conveyor &sub) : ConveyorAction(sub)
        {
            to_shooter_ = sub.getRobot().getSettingsParser().getDouble("conveyor:toshooter") ;
            to_collecter_ = sub.getRobot().getSettingsParser().getDouble("conveyor:tocollector") ;
            delay1_ = sub.getRobot().getSettingsParser().getDouble("conveyor:delay1") ;
        }

        ConveyorEmitAction::~ConveyorEmitAction()
        {
        }

        void ConveyorEmitAction::start()
        {
            state_ = State::Done ;
            setMotor(1.0) ;
            setMotor2(1.0) ;
        }

        void ConveyorEmitAction::run()
        {
            auto &sub = getSubsystem() ;

            switch(state_)
            {
            case State::WaitForBall:
                if (sub.getSensor1())
                {
                    //
                    // We have a ball, turn on the motor
                    //
                    setMotor(to_shooter_) ;
                    setMotor2(to_shooter_) ;
                    if (sub.getBallCount())
                    {
                        state_ = State::WaitForSecondOff ;
                    }
                    else
                    {
                        state_ = State::WaitForSecondOn ;
                    }
                    
                }
                break ;

            case State::WaitForSecondOff:
                if (sub.getSensor2() == false)
                {
                    state_ = State::WaitForSecondOn ;
                }
                break ;

            case State::WaitForSecondOn:
                if (sub.getSensor2() == true)
                {
                    state_ = State::WaitForSecondDelay ;
                    start_ = getSubsystem().getRobot().getTime() ;
                }
                break ;

            case State::WaitForSecondDelay:
                if (getSubsystem().getRobot().getTime() - start_ > delay1_)
                {
                    setMotor(0.0) ;
                    state_ = State::WaitForBall ;
                }

            case State::Done:
                break ;
            }
        }
    }
} 
