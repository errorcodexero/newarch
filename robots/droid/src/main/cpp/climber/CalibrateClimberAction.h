#pragma once

#include "ClimberAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <PIDCtrl.h>
#include <vector>

namespace xero
{
    namespace droid
    {
        class CalibrateClimberAction : public ClimberAction
        {
        public:
            CalibrateClimberAction(Climber &subsystem) ;
            virtual ~CalibrateClimberAction() ;
            virtual void start() ;
            virtual void run() ;
            virtual std::string toString() ;

        private:
            enum class State
            {
                DownSlowly,
                Holding
            } ;

        private:
            bool isStopped() ;

        private:
            State state_ ;
            double down_power_ ;
            double threshold_ ;
            std::vector<double> encoders_ ;
            size_t samples_ ;
            xero::misc::PIDCtrl holdpid_ ;
        } ;
    }
}