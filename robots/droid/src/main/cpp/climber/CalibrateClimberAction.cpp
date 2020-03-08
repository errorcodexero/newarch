#include "CalibrateClimberAction.h"
#include <droidids.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero
{
    namespace droid
    {
        CalibrateClimberAction::CalibrateClimberAction(Climber &subsystem) : ClimberAction(subsystem)
        {
            down_power_ = -0.20 ;
            samples_ = 5 ;
            threshold_ = 2.0 ;

            holdpid_.initFromSettingsExtended(subsystem.getRobot().getSettingsParser(), "climber:lifter:hold") ;
        }

        CalibrateClimberAction::~CalibrateClimberAction()
        {            
        }

        void CalibrateClimberAction::start()
        {
            ClimberAction::start() ;

            if (getSubsystem().calibrated_)
                state_ = State::Holding ;
            else
            {
                state_ = State::DownSlowly ;
                getSubsystem().getLifter()->getMotorController()->set(down_power_) ;
                getSubsystem().getLifter()->setDefaultAction(nullptr) ;
            }
        }

        void CalibrateClimberAction::run()
        {
            ClimberAction::run() ;

            switch(state_)
            {
                case State::DownSlowly:
                    encoders_.push_back(getSubsystem().getLifter()->getPosition()) ;
                    if (encoders_.size() > samples_)
                        encoders_.erase(encoders_.begin()) ;

                    if (isStopped())
                    {
                        getSubsystem().calibrated_ = true ;
                        getSubsystem().getLifter()->reset() ;
                        state_ = State::Holding ;
                    }
                    break ;

                case State::Holding:
                    {
                        double out = -0.05 ;
                        getSubsystem().getLifter()->getMotorController()->set(out) ;

                        auto &logger = getSubsystem().getRobot().getMessageLogger() ;
                        logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CLIMBER) ;
                        logger << "Climber holding at " << out ;
                        logger.endMessage() ;

                    }
                    break ;
            }
        }

        bool CalibrateClimberAction::isStopped()
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
            logger << "Climber calibration, delta " << vmax - vmin << " vs threshold " << threshold_ ;
            logger << " data" ;
            for(auto &d : encoders_)
                logger << " " << d ;
            logger.endMessage() ;

            return vmax - vmin < threshold_ ;
        }

        std::string CalibrateClimberAction::toString()
        {
            std::string ret = "CalibrateClimberAction " ;
            return ret ;
        }
    }
}
