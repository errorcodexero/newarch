#pragma once

#include "XeroTalonSRX.h"
#include <Subsystem.h>
#include <Encoder.h>
#include <I2C.h>
#include <DigitalInput.h>
#include <memory>

namespace xero {
    namespace bunny2018 {
		
        class Sorter : public xero::base::Subsystem {
            friend class SorterDutyCycleAction ;
            friend class SorterStageBallAction ;
            friend class SorterCalibrateAction ;

        public:
            enum BallColor
            {
                Red,
                Blue,
                None
            } ;

        public:
            Sorter(xero::base::Robot & robot);

            virtual ~Sorter(); 

            virtual void computeState() ;
		    virtual bool canAcceptAction(xero::base::ActionPtr action)  ;

            bool hasBall() const { return !(ball_ != BallColor::None) ; }
            BallColor getBallColor() const { return ball_ ; }

            double getAngle() const {
                return angle_ ;
            }

        private:
            void setIntakeMotor(double v) {
                inmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
            }

            void setOuttakeMotor(double v) {
                outmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
            }

            void setSorterMotor(double v) {
				sorter_motor_power_ = v ;
                sortmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
            }

            void setCalibrated(bool b) {
                calibrated_ = b ;
				if (b)
					calibrated_angle_ = angle_ ;
            }

            void detectBall() ;

			bool getIndexState() const {
				return index_state_ ;
			}

			double getSorterMotorPower() const {
				return sorter_motor_power_ ;
			}

        private:
            std::shared_ptr<frc::Encoder> encoder_ ;
            std::shared_ptr<TalonSRX> sortmotor_ ;
            std::shared_ptr<TalonSRX> inmotor_ ;
            std::shared_ptr<TalonSRX> outmotor_ ;
            std::shared_ptr<frc::I2C> color_sensor_ ;
            std::shared_ptr<frc::DigitalInput> ball_present_ ;
            std::shared_ptr<frc::DigitalInput> red_blue_ ;
			std::shared_ptr<frc::DigitalInput> index_ ;
            bool calibrated_ ;
            double degrees_per_tick_ ;

            BallColor ball_ ;
            double angle_ ;

			bool index_state_ ;

			double calibrated_angle_ ;

			double sorter_motor_power_ ;
        };
    }
}