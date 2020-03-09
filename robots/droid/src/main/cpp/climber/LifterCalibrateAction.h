#pragma once

#include "Lifter.h"
#include <motorencodersubsystem/MotorEncoderSubsystemAction.h>
#include <vector>

namespace xero
{
    namespace droid
    {
        class LifterCalibrateAction : public xero::base::MotorEncoderSubsystemAction
        {
        public:
            LifterCalibrateAction(Lifter &subsystem) ;
            virtual ~LifterCalibrateAction() ;
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
        } ;
    }
}