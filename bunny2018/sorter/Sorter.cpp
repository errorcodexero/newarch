#include "Sorter.h"
#include "SorterAction.h"
#include "bunnyids.h"
#include <Robot.h>
#include <xeromath.h>
#include <smartdashboard/SmartDashboard.h>
#include <memory>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
        Sorter::Sorter(xero::base::Robot & robot) :Subsystem(robot,"sorter"){
            auto &logger = robot.getMessageLogger() ;

            int sortmotor=robot.getSettingsParser().getInteger("hw:sorter:motor");                        
            int enc1=robot.getSettingsParser().getInteger("hw:sorter:encoder1");
            int enc2=robot.getSettingsParser().getInteger("hw:sorter:encoder2");
            int sensoraddr = robot.getSettingsParser().getInteger("hw:sorter:sensoraddr") ;
			int index = robot.getSettingsParser().getInteger("hw:sorter:index") ;

			white_detect_threshold_ = robot.getSettingsParser().getInteger("sorter:ball_detect:white_threshold") ;

            color_ = std::make_shared<TCS34725ColorSensor>(sensoraddr, TCS34725ColorSensor::TCS34725_INTEGRATIONTIME_24MS, TCS34725ColorSensor::TCS34725_GAIN_4X) ;
            if (!color_->isAlive()) {
                color_ = nullptr ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "cannot communicate with the color sensor" ;
                logger.endMessage() ;

                std::cout << "color sensor failed" << std::endl ;
            }
            else {
				if (!color_->initialize()) {
					logger.startMessage(MessageLogger::MessageType::error) ;
					logger << "cannot initialize color sensor" ;
					logger.endMessage() ;
				}

                if (!color_->enable()) {
					logger.startMessage(MessageLogger::MessageType::error) ;
					logger << "cannot enable color sensor" ;
					logger.endMessage() ;					
				}
            }

            sortmotor_ = std::make_shared<TalonSRX>(sortmotor);
	        encoder_ = std::make_shared<frc::Encoder>(enc1,enc2);

            degrees_per_tick_ = robot.getSettingsParser().getDouble("sorter:degrees_per_tick");

			sorter_motor_power_ = 0.0 ;
        }

        Sorter::~Sorter(){
        }

        void Sorter::detectBall(uint16_t &red, uint16_t &green, uint16_t &blue, uint16_t &white) {
			if (color_ == nullptr)
				ball_ = BallColor::None ;
			else {
				color_->getRawData(red, green, blue, white) ;

				if (white > white_detect_threshold_) {
					if (red > green && red > blue) {
						ball_ = BallColor::Red ;
					}
					else if (blue > red && blue > green) {
						ball_ = BallColor::Blue ;
					}
					else {
						ball_ = BallColor::None ;
					}
				}
				else {
					ball_ = BallColor::None ;
				}
			}
        }
    
        void Sorter::computeState() {
			uint16_t red, green, blue, white ;
            auto &logger = getRobot().getMessageLogger() ;

            detectBall(red, green, blue, white) ;
			
			ticks_ = encoder_->Get() ;
            angle_ = xero::math::normalizeAngleDegrees(ticks_ * degrees_per_tick_) ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER_VERBOSE) ;
            logger << "Sorter: \n" ;
			logger << "    angle " << angle_  << "\n" ;
			logger << "    sorter motor " << sorter_motor_power_ << "\n" ;
            logger << "    encoder " << encoder_->Get() << "\n" ;
			logger << "    ball " << toString(ball_) << "\n" ;
			logger << "    rgbw " << red << " " << green << " " << blue << " " << white ;
            logger.endMessage() ;

			frc::SmartDashboard::PutString("BallColor", toString(ball_)) ;

        }
    
		bool Sorter::canAcceptAction(ActionPtr action) {
			std::shared_ptr<SorterAction> act_p = std::dynamic_pointer_cast<SorterAction>(action) ;
			if (act_p == nullptr)
				return false ;

			return true ;
		}
    }
}