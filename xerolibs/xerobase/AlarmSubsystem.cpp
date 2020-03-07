#include "AlarmSubsystem.h"
#include "Robot.h"
#include "oi/OISubsystem.h"
#include <MessageLogger.h>
#include <map>
#include <cassert>

namespace xero {
    namespace base {

        AlarmSubsystem::AlarmSubsystem(Subsystem *parent) : Subsystem(parent, "Alarms")
        {
        }

        AlarmSubsystem::~AlarmSubsystem()
        {
        }

        void AlarmSubsystem::setAlarm(LoopType ltype, double remaining)
        {
            assert(ltype == LoopType::OperatorControl) ;
            alarms_.push_back(std::make_pair(ltype, remaining)) ;
        }

        void AlarmSubsystem::run()
        {
            size_t i = 0 ;
            double remaining = frc::DriverStation::GetInstance().GetMatchTime() ;
            auto oi = getRobot().getOI() ;
            auto gamepad = oi->getDriverGamepad() ;

            // For now, this only work in teleop
            if (!getRobot().IsOperatorControl())
                return ;

            while (i < alarms_.size())
            {
                if (remaining < alarms_[i].second)
                {
                    //
                    // This alarm needs to go off
                    //
                    alarms_.erase(alarms_.begin() + i) ;
                    gamepad->rumble(false, 1.0, 5.0) ;
                }
                else
                {
                    i++ ;
                }
            }
        }
    }
}
