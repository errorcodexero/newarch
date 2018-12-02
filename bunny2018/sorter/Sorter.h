#pragma once

#include "XeroTalonSRX.h"
#include "ColorSensor.h"
#include <VictorSP.h>
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
            friend class SorterEjectAction ;


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
#ifdef USE_VICTORS
                inmotor_->Set(v) ;
#else 
                inmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
#endif
            }

            void setOuttakeMotor(double v) {
#ifdef USE_VICTORS
                outmotor_->Set(v) ;
#else                
                outmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
#endif
            }

            void setSorterMotor(double v) {
				sorter_motor_power_ = v ;
#ifdef USE_VICTORS
                sortmotor_->Set(v) ;
#else
                sortmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
#endif
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

#ifdef USE_VICTORS
            std::shared_ptr<VictorSP> sortmotor_ ;
            std::shared_ptr<VictorSP> inmotor_ ;
            std::shared_ptr<VictorSP> outmotor_ ;
#else
            std::shared_ptr<TalonSRX> sortmotor_ ;
            std::shared_ptr<TalonSRX> inmotor_ ;
            std::shared_ptr<TalonSRX> outmotor_ ;
#endif
            std::shared_ptr<frc::DigitalInput> ball_present_ ;
            std::shared_ptr<frc::DigitalInput> red_blue_ ;
			std::shared_ptr<frc::DigitalInput> index_ ;

            std::shared_ptr<ColorSensor> color_ ;

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