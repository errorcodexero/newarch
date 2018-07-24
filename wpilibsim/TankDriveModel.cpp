#include "TankDriveModel.h"
#include <algorithm>

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
            right_rps_per_volt_per_time_ = 9.16;
            left_rps_per_volt_per_time_ = 9.16; 
        	time_interval_ = 100000.0;
	        last_output_ = 0.0;                       
        }

        TankDriveModel::~TankDriveModel() {
        }

		std::string TankDriveModel::toString() {
			std::string result("TankDrive: ") ;

			result += "left " + std::to_string(left_) ;
			result += ", right " + std::to_string(right_) ;
			result += ", angle " + std::to_string(angle_) ;

			return result ;
		}

        void TankDriveModel::run(double dt) {
			double dleft = left_volts_ * left_rps_per_volt_per_time_ * dt * diameter_ * PI;
			double dright = right_volts_ * right_rps_per_volt_per_time_ * dt * diameter_ * PI;			

			left_ += dleft;
			right_ += dright;

			double dv = (dright - dleft) / 2 * scrub_;
			angle_ += (dv * 2.0) / width_;

			double lrevs = left_ / (PI * diameter_);
			double rrevs = right_ / (PI * diameter_) ;

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

				auto itr = std::find(std::begin(left_motors_), std::end(left_motors_), talon) ;
				if (itr != std::end(right_motors_)) {
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
    }
}
