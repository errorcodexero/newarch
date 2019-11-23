#include "LimeLight.h"
#include "CameraChangeAction.h"
#include "Robot.h"
#include "basegroups.h"
#include <networktables/NetworkTableInstance.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        LimeLight::LimeLight(Subsystem *parent) : Subsystem(parent, "LimeLight")
        {
            led_mode_ = ledMode::Invalid ;
            cam_mode_ = camMode::Invalid ;
        }

        LimeLight::~LimeLight()
        {            
        }

        void LimeLight::computeState()
        {            
            if(cam_mode_ == camMode::VisionProcessor)
            {
                std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight");
                if(table->ContainsKey("tv"))
                {
                    present_ = true ;
                    
                    if(std::fabs(table->GetNumber("tv", 0.0)) < 0.01)
                        tv_ = false ;
                    else
                    {
                        tx_ = table->GetNumber("tx",0.0);
                        ty_ = table->GetNumber("ty",0.0);
                        ta_ = table->GetNumber("ta",0.0);
                    }
                }
                else
                {
                    present_ = false ;
                    auto &logger=getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "did not detect LimeLight" ;
                    logger.endMessage() ;
                }
            }
            else
            {
                tv_ = false ;
            }
            

        }

        void LimeLight::run()
        {            
        }

        bool LimeLight::canAcceptAction(ActionPtr action)
        {      
            return false ;      
        }

        void LimeLight::setCamMode(camMode mode)
        {
            if (cam_mode_ != mode)
            {
                cam_mode_ = mode ;

                switch(mode)
                {
                case camMode::VisionProcessor:
                    table_->PutString(camModeName, "0") ;
                    break ;
                case camMode::DriverCamera:
                    table_->PutString(camModeName, "1") ;                
                    break ;
                case camMode::Invalid:
                    break ;
                }
            }

        }

        void LimeLight::setLedMode(ledMode mode)
        {
            if (led_mode_ != mode)
            {
                led_mode_ = mode ;

                switch(mode){
                    case ledMode::UseLED:
                        table_->PutNumber(ledModeName, 0) ;
                        break ;
                    case ledMode::ForceOff:
                        table_->PutNumber(ledModeName, 1) ;
                        break ;
                    case ledMode::ForceBlink:
                        table_->PutNumber(ledModeName, 2) ;
                        break ;
                    case ledMode::ForceOn:
                        table_->PutNumber(ledModeName, 3) ;
                        break ;
                    case ledMode::Invalid:
                        break ;
                }
            }
        }

        std::string LimeLight::toString() {
             return "LimeLight" ;
        }
    }
}