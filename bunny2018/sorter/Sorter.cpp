#include "Sorter.h"
#include "SorterAction.h"
#include "bunnyids.h"
#include <Robot.h>
#include <xeromath.h>
#include <memory>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
        Sorter::Sorter(xero::base::Robot & robot) :Subsystem(robot,"sorter"){
            auto &logger = robot.getMessageLogger() ;

            int inmotor=robot.getSettingsParser().getInteger("hw:sorter:in:motor");
            int outmotor=robot.getSettingsParser().getInteger("hw:sorter:out:motor");
            int sortmotor=robot.getSettingsParser().getInteger("hw:sorter:motor");                        
            int enc1=robot.getSettingsParser().getInteger("hw:sorter:encoder1");
            int enc2=robot.getSettingsParser().getInteger("hw:sorter:encoder2");
            int sensoraddr = robot.getSettingsParser().getInteger("hw:sorter:sensoraddr") ;
			int index = robot.getSettingsParser().getInteger("hw:sorter:index") ;

            color_ = std::make_shared<ColorSensor>(sensoraddr, ColorSensor::TCS34725_INTEGRATIONTIME_24MS, ColorSensor::TCS34725_GAIN_4X) ;
            if (!color_->isAlive()) {
                color_ = nullptr ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "cannot initialize the color sensor" ;
                logger.endMessage() ;

                std::cout << "color sensor failed" << std::endl ;
            }
            else {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
                logger << "color sensor has been installed";
                logger.endMessage() ;
                std::cout << "    - color sensor has been created" << std::endl ;

                if (!color_->enable())
                    std::cout << "Could not enable color sensor" << std::endl ;
                else
                    std::cout << "    - Color sensor has been enabled" << std::endl ;

                while (1) {
                    uint16_t r, g, b, c ;
                    color_->getRawData(&r, &g, &b, &c) ;

                    frc::Wait(10.0) ;
                    std::cout << "Colors: " << r << " " << g << " " << b << " " << c << std::endl ;
                }
            }

#ifdef USE_VICTORS
            sortmotor_ = std::make_shared<VictorSP>(sortmotor);
            inmotor_ = std::make_shared<VictorSP>(inmotor);
            outmotor_ = std::make_shared<VictorSP>(outmotor) ;
#else
            sortmotor_ = std::make_shared<TalonSRX>(sortmotor);
            inmotor_ = std::make_shared<TalonSRX>(inmotor);
            outmotor_ = std::make_shared<TalonSRX>(outmotor) ;
#endif
            encoder_ = std::make_shared<frc::Encoder>(enc1,enc2);
            encoder_->Reset() ;
			index_ = std::make_shared<frc::DigitalInput>(index) ;

            degrees_per_tick_ = robot.getSettingsParser().getDouble("sorter:degrees_per_tick");

            calibrated_ = true;

			sorter_motor_power_ = 0.0 ;
        }

        Sorter::~Sorter(){
        }

        void Sorter::detectBall() {
            uint16_t r, g, b, c ;
            color_->getRawData(&r, &g, &b, &c) ;

            std::cout << "RGB " << r << " " << g << " " << b << std::endl ;
        }
    
        void Sorter::computeState() {
            auto &logger = getRobot().getMessageLogger() ;

            detectBall() ;

            if (calibrated_)
                angle_ = xero::math::normalizeAngleDegrees(encoder_->Get() * degrees_per_tick_ - calibrated_angle_) ;
            else
                angle_ = 1000.0 ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
            logger << "The angle is " << angle_ ;
            logger << ", encoder value is " << encoder_->Get() ;
            logger.endMessage() ;

			index_state_ = index_->Get() ;
        }
    
		bool Sorter::canAcceptAction(ActionPtr action) {
			std::shared_ptr<SorterAction> act_p = std::dynamic_pointer_cast<SorterAction>(action) ;
			if (act_p == nullptr)
				return false ;

			return true ;
		}
    }
}