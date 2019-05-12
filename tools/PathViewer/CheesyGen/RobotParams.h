#pragma once

namespace xero {
	namespace paths
	{
		class RobotParams
		{
		public:
			RobotParams() {
			}

			virtual ~RobotParams() {
			}

			double getWidth() const {
				return width_;
			}

			void setWidth(double v) {
				width_ = v;
			}

			double getLength() const {
				return length_;
			}

			void setLength(double v) {
				length_ = v;
			}

			double getMaxVelocity() const {
				return max_velocity_;
			}

			void setMaxVelocity(double v) {
				max_velocity_ = v;
			}

			double getMaxAcceleration() const {
				return max_acceleration_;
			}

			void setMaxAcceleration(double v) {
				max_acceleration_ = v;
			}

			double getMaxJerk() const {
				return max_jerk_;
			}

			void setMaxJerk(double v) {
				max_jerk_ = v;
			}

		private:
			double width_;
			double length_;
			double max_velocity_;
			double max_acceleration_;
			double max_jerk_;
		};
	}
}

