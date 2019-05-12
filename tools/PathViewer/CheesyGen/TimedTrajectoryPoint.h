#pragma once

#include "TrajectorySamplePoint.h"

namespace xero {
	namespace paths {
		class TimedTrajectoryPoint : public TrajectorySamplePoint
		{
		public:
			TimedTrajectoryPoint(const TrajectorySamplePoint& pt, double t, double v, double a);
			virtual ~TimedTrajectoryPoint();

			double getTime() const {
				return time_;
			}

			void setTime(double t) {
				time_ = t;
			}

			double getVelocity() const {
				return velocity_;
			}

			void setVelocity(double v) {
				velocity_ = v;
			}

			double getAcceleration() const {
				return acceleration_;
			}

			void setAcceleration(double a) {
				acceleration_ = a;
			}

			virtual std::string toCSV() const {
				std::string ret;
				ret += std::to_string(time_);
				ret += "," + Pose2dWithCurvature::toCSV();
				ret += "," + std::to_string(velocity_);
				ret += "," + std::to_string(acceleration_);

				return ret;
			}

		private:
			double time_;
			double velocity_;
			double acceleration_;
		};
	}
}

