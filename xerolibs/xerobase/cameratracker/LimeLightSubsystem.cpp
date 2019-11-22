#include "LimeLightSubsystem.h"
#include "CameraChangeAction.h"
#include "Robot.h"
#include "basegroups.h"
#include <networktables/NetworkTableInstance.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {
        LimeLightSubsystem::LimeLightSubsystem(Subsystem *parent) : Subsystem(parent, "LimeLightSubsystem")
        {
            nt::NetworkTableInstance ntinst = nt::NetworkTableInstance::GetDefault() ;
            table_ = ntinst.GetTable(NetworkTableName) ;

            mode_ = CameraMode::Invalid ;
            led_ = LEDMode::Invalid ;
            pipeline_ = -1 ;
        }

        LimeLightSubsystem::~LimeLightSubsystem()
        {            
        }

        void LimeLightSubsystem::computeState()
        {
            MessageLogger &logger = getRobot().getMessageLogger() ;            
            double tmp ;

            if (!table_->ContainsKey(TV))
            {
                present_ = false ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "LimeLightSubsystem: no limelight device detected" ;
            }
            else
            {
                present_ = true ;
                tmp = table_->GetNumber(TV, 0.0) ;
                if (std::fabs(tmp - 1.0) < 0.1)
                    tv_ = true ;

                ta_ = table_->GetNumber(TA, 0.0) ;
                tx_ = table_->GetNumber(TX, 0.0) ;
                ty_ = table_->GetNumber(TY, 0.0) ;

            }
        }

        void LimeLightSubsystem::run()
        {       
            MessageLogger &logger = getRobot().getMessageLogger() ;    
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CAMERA_TRACKER_VERBOSE) ;
            logger << "LimeLightSubsystem" ;
            logger << " tv " << tv_ ;
            logger << " ta " << ta_ ;
            logger << " tx " << tx_ ;
            logger << " ty " << ty_ ;
            logger.endMessage() ;                 
        }

        bool LimeLightSubsystem::canAcceptAction(ActionPtr action)
        {            
            std::shared_ptr<CameraChangeAction> camact = std::dynamic_pointer_cast<CameraChangeAction>(action) ;
            return camact != nullptr ;
        }

        void LimeLightSubsystem::setMode(LimeLightSubsystem::CameraMode mode)
        {
            if (mode_ != mode)
            {
                mode_ = mode ;
                switch(mode)
                {
                case CameraMode::TargetTracking:
                    table_->PutNumber(CAMMODE, 0.0) ;
                    break ;
                case CameraMode::DriverViewing:
                    table_->PutNumber(CAMMODE, 1.0) ;
                    break ;
                case CameraMode::Invalid:
                    break ;
                }
            }
        }

        void LimeLightSubsystem::setLED(LimeLightSubsystem::LEDMode led)
        {
            if (led != led_)
            {
                led_ = led ;
                switch(led)
                {
                    case LEDMode::Pipeline:
                        table_->PutNumber(LEDMODE, 0.0) ;
                        break ;                    
                    case LEDMode::Off:
                        table_->PutNumber(LEDMODE, 1.0) ;                    
                        break ;                    
                    case LEDMode::Blink:
                        table_->PutNumber(LEDMODE, 2.0) ;                    
                        break ;                    
                    case LEDMode::On:
                        table_->PutNumber(LEDMODE, 3.0) ;                    
                        break ;                    
                    case LEDMode::Invalid:
                        break ;
                }
            }
        }

        void LimeLightSubsystem::setPipeline(int pipeline)
        {
            assert(pipeline >= 0 && pipeline <= 9) ;
            if (pipeline != pipeline_)
            {
                pipeline_ = pipeline ;
                table_->PutNumber(PIPELINE, (double)pipeline) ;
            }
        }
    }
}
