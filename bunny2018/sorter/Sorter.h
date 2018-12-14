#pragma once

#include "XeroTalonSRX.h"
#include "TCS34725ColorSensor.h"
#include <VictorSP.h>
#include <Subsystem.h>
#include <Encoder.h>
#include <I2C.h>
#include <DigitalInput.h>
#include <memory>

namespace xero {
    namespace bunny2018 {
		
        class Sorter : public xero::base::Subsystem {
            friend class SorterPowerAction ;
            friend class SorterEjectAction ;
			friend class SorterTestAlignAction ;
            friend class SorterSortAction ;

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

            bool hasBall() const { return !(ball_ == BallColor::None) ; }
            BallColor getBallColor() const { return ball_ ; }

            double getAngle() const {
                return angle_ ;
            }

			int getTicks() const {
				return ticks_ ;
			}

        private:
            void setSorterMotor(double v) {
				sorter_motor_power_ = v ;
                sortmotor_->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v) ;
            }

            void detectBall(uint16_t &red, uint16_t &green, uint16_t &blue, uint16_t &white) ;

			double getSorterMotorPower() const {
				return sorter_motor_power_ ;
			}

			std::string toString(BallColor c) {
				std::string ret ;

				switch(c) {
				case BallColor::None:
					ret = "None" ;
					break ;

				case BallColor::Red:
					ret = "Red" ;
					break ;

				case BallColor::Blue:
					ret = "Blue" ;
					break ;

				default:
					ret = std::to_string(static_cast<int>(c)) ;
					break ;
				}

				return ret ;
			}

        private:
            std::shared_ptr<frc::Encoder> encoder_ ;
            std::shared_ptr<TalonSRX> sortmotor_ ;
            std::shared_ptr<xero::base::TCS34725ColorSensor> color_ ;

            double degrees_per_tick_ ;

            BallColor ball_ ;
            double angle_ ;
			int ticks_ ;


			double sorter_motor_power_ ;

			int white_detect_threshold_;
        };
    }
}