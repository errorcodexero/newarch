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

            int inmotor=robot.getSettingsParser().getInteger("hw:sorter:in:motor");
            int outmotor=robot.getSettingsParser().getInteger("hw:sorter:out:motor");
            int sortmotor=robot.getSettingsParser().getInteger("hw:sorter:motor");                        
            int enc1=robot.getSettingsParser().getInteger("hw:sorter:encoder1");
            int enc2=robot.getSettingsParser().getInteger("hw:sorter:encoder2");
            int sensoraddr = robot.getSettingsParser().getInteger("hw:sorter:sensoraddr") ;
			int index = robot.getSettingsParser().getInteger("hw:sorter:index") ;

			white_detect_threshold_ = robot.getSettingsParser().getInteger("sorter:ball_detect:white_threshold") ;
			blue_detect_threshold_ = robot.getSettingsParser().getInteger("sorter:ball_detect:blue_threshold") ;
			red_detect_threshold_ = robot.getSettingsParser().getInteger("sorter:ball_detect:red_threshold") ;

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
			calibrated_angle_ = -36.0 ;
			sorter_motor_power_ = 0.0 ;
        }

        Sorter::~Sorter(){
        }

        void Sorter::detectBall(uint16_t &red, uint16_t &green, uint16_t &blue, uint16_t &white) {
			color_->getRawData(red, green, blue, white) ;

			if (white > white_detect_threshold_) {
				if (red > green && red > blue && red > red_detect_threshold_) {
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
    
        void Sorter::computeState() {
			uint16_t red, green, blue, white ;
            auto &logger = getRobot().getMessageLogger() ;

            detectBall(red, green, blue, white) ;
			
            if (calibrated_)
                angle_ = xero::math::normalizeAngleDegrees(encoder_->Get() * degrees_per_tick_ - calibrated_angle_) ;
            else
                angle_ = 1000.0 ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
            logger << "Sorter: angle " << angle_  << "\n" ;
            logger << "        encoder " << encoder_->Get() << "\n" ;
			logger << "        ball " << toString(ball_) << "\n" ;
			logger << "        rgbw " << red << " " << green << " " << blue << " " << white ;
            logger.endMessage() ;

			frc::SmartDashboard::PutString("BallColor", toString(ball_)) ;

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