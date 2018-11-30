#include "Sorter.h"
#include "SorterAction.h"
#include <Robot.h>
#include <xeromath.h>

using namespace xero::base;

namespace xero {
    namespace bunny2018 {
        Sorter::Sorter(xero::base::Robot & robot) :Subsystem(robot,"sorter"){
            int inmotor=robot.getSettingsParser().getInteger("hw:sorter:in:motor");
            int outmotor=robot.getSettingsParser().getInteger("hw:sorter:out:motor");
            int sortmotor=robot.getSettingsParser().getInteger("hw:sorter:motor");                        
            int enc1=robot.getSettingsParser().getInteger("hw:sorter:encoder1");
            int enc2=robot.getSettingsParser().getInteger("hw:sorter:encoder2");
            int sensoraddr = robot.getSettingsParser().getInteger("hw:sorter:sensoraddr") ;
			int index = robot.getSettingsParser().getInteger("hw:sorter:index") ;

            enc1 = 0 ;
            enc2 = 1 ;

            if (robot.getSettingsParser().isDefined("hw:sorter:ballpresent") && 
                            robot.getSettingsParser().isDefined("hw:sorter:ballcolor")) {
                int present = robot.getSettingsParser().getInteger("hw:sorter:ballpresent");
                int color = robot.getSettingsParser().getInteger("hw:sorter:ballcolor");

                ball_present_ = std::make_shared<frc::DigitalInput>(present) ;
                red_blue_ = std::make_shared<frc::DigitalInput>(color) ;
            }
            else {
                color_sensor_ = std::make_shared<frc::I2C>(frc::I2C::kOnboard, sensoraddr) ;
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
			index_ = std::make_shared<frc::DigitalInput>(index) ;

            degrees_per_tick_ = robot.getSettingsParser().getDouble("sorter:degrees_per_tick");

            calibrated_ = false;

			sorter_motor_power_ = 0.0 ;
        }

        Sorter::~Sorter(){
        }

        void Sorter::detectBall() {
            if (color_sensor_ == nullptr) {
                //
                // If not color sensor is defined, we are relying on two digital
                // inputs.  One is active when a ball is present.  If a ball is present
                // the secone gives us the color.
                //
                if (ball_present_->Get()) {
                    if (red_blue_->Get())
                        ball_ = BallColor::Red ;
                    else
                        ball_ = BallColor::Blue ;
                }
                else {
                    ball_ = BallColor::None ;
                }
            }
            else {
                //
                // Logic to read I2C to get red/blue/present data
                //
            }
        }
    
        void Sorter::computeState() {
            detectBall() ;

            angle_ = xero::math::normalizeAngleDegrees(encoder_->Get() * degrees_per_tick_ - calibrated_angle_) ;

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