#include "TankDriveModel.h"
#include <algorithm>
#include <cmath>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        TankDriveModel::TankDriveModel() {
            left_ = 0.0;
            right_ = 0.0;
            angle_ = 0.0;
            ticks_per_rev_ = 200;
            diameter_ = 5.0;
            left_volts_ = 0.0;
            right_volts_ = 0.0;
            scrub_ = 1.0;
            width_ = 24.0;
            right_rps_per_volt_per_time_ = 7.63 ;
            left_rps_per_volt_per_time_ = 7.63 ;
        	time_interval_ = 100000.0;
	        last_output_ = 0.0;            
			xpos_ = 0.0 ;
			ypos_ = 0.0 ;           
        }

        TankDriveModel::~TankDriveModel() {
        }

		std::string TankDriveModel::toString() {
			std::string result("TankDrive: ") ;

			result += "left " + std::to_string(left_) ;
			result += ", right " + std::to_string(right_) ;
			result += ", angle " + std::to_string(angle_) ;
			result += ", xpos " + std::to_string(xpos_) ;
			result += ", ypos " + std::to_string(ypos_) ;

			return result ;
		}

        void TankDriveModel::run(double dt) {
			double dleft = left_volts_ * left_rps_per_volt_per_time_ * dt * diameter_ * PI;
			double dright = right_volts_ * right_rps_per_volt_per_time_ * dt * diameter_ * PI;

			left_ += dleft;
			right_ += dright;

			double dv = (dright - dleft) / 2 * scrub_;

			double lrevs = left_ / (PI * diameter_);
			double rrevs = right_ / (PI * diameter_) ;

			updatePosition(dleft, dright, angle_) ;
			angle_ += (dv * 2.0) / width_;

			if (left_enc_ != nullptr)
				left_enc_->SimulatorSetValue(static_cast<int32_t>(lrevs * ticks_per_rev_));

			if (right_enc_ != nullptr)
				right_enc_->SimulatorSetValue(static_cast<int32_t>(rrevs * ticks_per_rev_));			

			if (navx_ != nullptr)
				navx_->SimulatorSetYaw(angle_) ;			
        }

		void TankDriveModel::inputChanged(SimulatedObject *obj) {
			std::lock_guard<std::mutex> lock(getLockMutex()) ;

			TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
			if (talon != nullptr) {
				auto itl = std::find(std::begin(left_motors_), std::end(left_motors_), talon) ;
				if (itl != std::end(left_motors_)) {
					left_volts_ = talon->Get() ;
				}

				auto itr = std::find(std::begin(right_motors_), std::end(right_motors_), talon) ;
				if (itr != std::end(right_motors_)) {
					right_volts_ = talon->Get() ;
				}
			}

			if (obj == left_enc_) {
				left_ = 0.0 ;
			}

			if (obj == right_enc_) {
				right_ = 0.0 ;
			}

			if (obj == navx_) {
				angle_ = 0.0 ;
			}
		}

		void TankDriveModel::addTalon(TalonSRX *motor) {
			if (motor->GetDeviceID() == 1 || motor->GetDeviceID() == 2 || motor->GetDeviceID() == 3) {
				left_motors_.push_back(motor) ;
				motor->addModel(this) ;
			}
			else if (motor->GetDeviceID() == 14 || motor->GetDeviceID() == 15 || motor->GetDeviceID() == 16) {
				right_motors_.push_back(motor) ;
				motor->addModel(this) ;
			}
		}

		void TankDriveModel::addEncoder(Encoder *encoder) {
			int first, second ;
			encoder->SimulatorGetDigitalIOs(first, second) ;
			if (first == 0 && second == 1) {
				left_enc_ = encoder ;
				left_enc_->addModel(this) ;
			}
			else if (first == 2 && second == 2) {
				right_enc_ = encoder ;
				right_enc_->addModel(this) ;
			}
		}

		void TankDriveModel::addNavX(AHRS *navx) {
			navx_ = navx ;
			navx_->addModel(this) ;
		}

		void TankDriveModel::updatePosition(double dl, double dr, double angle) {
			if (std::fabs(dl - dr) < 1e-6) {
				// Straight line
				xpos_ += dl * std::cos(angle) ;
				ypos_ += dr * std::sin(angle) ;
			}
			else {
				double r = width_ * (dl + dr) / (2 * (dr - dl)) ;
				double wd = (dr - dl) / width_ ;
				xpos_ = xpos_ + r * std::sin(wd + angle) - r * std::sin(angle) ;
				ypos_ = ypos_ - r * std::cos(wd + angle) + r * std::cos(angle) ;
			}
		}
    }
}
