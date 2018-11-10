#pragma once

#include <xeromath.h>

namespace xero {
	namespace misc {
		class Speedometer {
		public:
			Speedometer(bool angle = false) {
				angle = angle_ ;
				dist_ = 0.0 ;
				last_dist_ = 0.0 ;
				velocity_ = 0.0 ;
				last_velocity_ = 0.0 ;
				accel_ = 0.0 ;
			}

			double getDistance() const {
				return dist_ ;
			}

			double getVelocity() const {
				return velocity_ ;
			}

			double getAcceleration() const {
				return accel_ ;
			}

			void update(double dt, double pos) {
				dist_ = pos ;
				if (angle_)
					velocity_ = xero::math::normalizeAngleDegrees(dist_ - last_dist_) / dt ;
				else
					velocity_ = (dist_ - last_dist_) / dt ;
					
				accel_ = (velocity_ - last_velocity_) / dt ;

				last_dist_ = dist_ ;
				last_velocity_ = velocity_ ;
			}

		private:
			double dist_ ;
			double last_dist_ ;
			double velocity_ ;
			double last_velocity_ ;
			double accel_ ;
			bool angle_ ;
		} ;
	}
}