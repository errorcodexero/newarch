#include "BlinkySubsystem.h"
#include "BlinkyAction.h"
#include <Robot.h>

namespace xero
{
    namespace droid
    {
        BlinkySubsystem::BlinkySubsystem(xero::base::Subsystem *parent) : Subsystem(parent, "blinky")
        {
            int rnum ;

            rnum = getRobot().getSettingsParser().getInteger("hw:blinky:r0") ;
            r0_ = std::make_shared<frc::Relay>(rnum) ;

            rnum = getRobot().getSettingsParser().getInteger("hw:blinky:r1") ;
            r1_ = std::make_shared<frc::Relay>(rnum) ;
        }

        BlinkySubsystem::~BlinkySubsystem()
        {            
        }

        void BlinkySubsystem::init(xero::base::LoopType ltype)
        {
            setPattern(0) ;
        }

        void BlinkySubsystem::computeMyState() 
        {            
        }

        void BlinkySubsystem::run()
        {            
        }

        bool BlinkySubsystem::canAcceptAction(xero::base::ActionPtr action)
        {
            auto ptr = std::dynamic_pointer_cast<BlinkyAction>(action) ;
            return ptr != nullptr ;
        }

        void BlinkySubsystem::setPattern(uint8_t pattern)
        {
            switch((pattern >> 0) & 0x03)
            {
                case 0:
                    r0_->Set(frc::Relay::kOff) ;
                    break ;                
                case 1:
                    r0_->Set(frc::Relay::kForward) ;                
                    break ;                
                case 2:
                    r0_->Set(frc::Relay::kReverse) ;                
                    break ;                
                case 3:
                    r0_->Set(frc::Relay::kOn) ;                
                    break ;
            }

            switch((pattern >> 2) & 0x03)
            {
                case 0:
                    r1_->Set(frc::Relay::kOff) ;
                    break ;                
                case 1:
                    r1_->Set(frc::Relay::kForward) ;                
                    break ;                
                case 2:
                    r1_->Set(frc::Relay::kReverse) ;                
                    break ;                
                case 3:
                    r1_->Set(frc::Relay::kOn) ;                
                    break ;
            }
        }
    }
}
