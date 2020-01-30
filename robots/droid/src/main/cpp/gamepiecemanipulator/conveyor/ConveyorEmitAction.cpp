#include "ConveyorEmitAction.h"
#include "Conveyor.h"

using namespace xero::misc;

namespace xero {
    namespace droid {
        ConveyorEmitAction::ConveyorEmitAction(Conveyor &sub) : ConveyorAction(sub)
        {
            to_shooter_ = 1.0 ;
            to_collecter_ = -1.0 ;
        }

        ConveyorEmitAction::~ConveyorEmitAction()
        {
        }

        void ConveyorEmitAction::start()
        {
            state_ = State::WaitForBall ;
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
                setMotor(0.0) ;
                break ;
            }
        }
    }
} 
