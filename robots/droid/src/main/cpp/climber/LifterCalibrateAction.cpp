#include "LifterCalibrateAction.h"
#include "Lifter.h"
#include <droidids.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero
{
    namespace droid
    {
        LifterCalibrateAction::LifterCalibrateAction(Lifter &subsystem) : MotorEncoderSubsystemAction(subsystem)
        {
            down_power_ = -0.20 ;
            samples_ = 5 ;
            threshold_ = 2.0 ;
        }

        LifterCalibrateAction::~LifterCalibrateAction()
        {            
        }

        void LifterCalibrateAction::start()
        {
            Lifter &lifter = dynamic_cast<Lifter &>(getSubsystem()) ;
            MotorEncoderSubsystemAction::start() ;

            if (lifter.isCalibrated())
                state_ = State::Holding ;
            else
            {
                state_ = State::DownSlowly ;
                lifter.getMotorController()->set(down_power_) ;
            }
        }

        void LifterCalibrateAction::run()
        {
            Lifter &lifter = dynamic_cast<Lifter &>(getSubsystem()) ;

            switch(state_)
            {
                case State::DownSlowly:
                    encoders_.push_back(lifter.getPosition()) ;
                    if (encoders_.size() > samples_)
                        encoders_.erase(encoders_.begin()) ;

                    if (isStopped())
                    {
                        lifter.setCalibrated() ;
                        lifter.reset() ;
                        state_ = State::Holding ;
                    }
                    break ;

                case State::Holding:
                    {
                        double out = -0.05 ;
                        lifter.getMotorController()->set(out) ;

                        auto &logger = getSubsystem().getRobot().getMessageLogger() ;
                        logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CLIMBER) ;
                        logger << "Lifter holding at " << out ;
                        logger.endMessage() ;

                    }
                    break ;
            }
        }

        bool LifterCalibrateAction::isStopped()
        {
            if (encoders_.size() < samples_)
                return false ;

            double vmax = encoders_[0] ;
            double vmin = encoders_[0] ;

            for(size_t i = 1 ; i < samples_ ; i++)
            {
                if (encoders_[i] < vmin)
                    vmin = encoders_[i] ;

                if (encoders_[i] > vmax)
                    vmax = encoders_[i] ;
            }

            auto &logger = getSubsystem().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CLIMBER) ;
            logger << "Lifter calibration, delta " << vmax - vmin << " vs threshold " << threshold_ ;
            logger << " data" ;
            for(auto &d : encoders_)
                logger << " " << d ;
            logger.endMessage() ;

            return vmax - vmin < threshold_ ;
        }

        std::string LifterCalibrateAction::toString()
        {
            std::string ret = "LifterCalibrateAction " ;
            return ret ;
        }
    }
}
