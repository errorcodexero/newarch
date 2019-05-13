#pragma once

#include "Pose2d.h"
#include "TimingConstraint.h"
#include <string>
#include <vector>
#include <memory>

namespace xero
{
	namespace paths
	{
		class RobotPath
		{
		public:
			RobotPath(const std::string& name) {
				name_ = name;
				start_vel_ = 0.0;
				end_vel_ = 0.0;
				max_vel_ = 0.0;
				max_accel_ = 0.0;
			}

			virtual ~RobotPath() {
			}

			const std::string& getName() const {
				return name_;
			}

			void addPoint(const Pose2d& pt) {
				points_.push_back(pt);
			}

			size_t size() const {
				return points_.size();
			}

			const Pose2d& operator[](size_t index) const {
				return points_[index];
			}

			const std::vector<Pose2d>& getPoints() const {
				return points_;
			}

			void addTimingConstraint(std::shared_ptr<TimingConstraint> cstr) {
				constraints_.push_back(cstr);
			}

			const ConstraintList& getConstraints() const {
				return constraints_;
			}

			double getStartVelocity() const {
				return start_vel_;
			}

			void setStartVelocity(double v) {
				start_vel_ = v;
			}

			double getEndVelocity() const {
				return end_vel_;
			}

			void setEndVelocity(double v) {
				end_vel_ = v;
			}

			double getMaxVelocity() const {
				return max_vel_;
			}

			void setMaxVelocity(double v) {
				max_vel_ = v;
			}

			double getMaxAccel() const {
				return max_accel_;
			}

			void setMaxAccel(double a) {
				max_accel_ = a;
			}

		private:
			std::string name_;
			std::vector<Pose2d> points_;
			ConstraintList constraints_;
			double start_vel_;
			double end_vel_;
			double max_vel_;
			double max_accel_;
		};
	}
}

