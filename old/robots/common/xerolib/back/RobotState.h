#pragma once

#include "PositionCS.h"
#include "PositionAngle.h"
#include <utility>
#include <vector>
#include <mutex>

namespace xero
{
	namespace pathfinder
	{
		class RobotState
		{
		private:
			RobotState();

		public:
			virtual ~RobotState();

			void reset(double start, const xero::math::PositionCS &pos);
			void reset(const xero::math::PositionCS &pos);

			void resetDrivenDistance()
			{
				std::lock_guard<std::mutex> lock(m_lock);
				m_driven_distance = 0;
			}

			static RobotState &get()
			{
				if (TheOneObject == nullptr)
					TheOneObject = new RobotState();

				return *TheOneObject;
			}

			const xero::math::PositionCS &getLatestFieldToVehicle() const
			{
				return m_actual_positions.back().second;
			}

			xero::math::PositionCS getPositionAtTime(double t);
			double getDrivenDistance();
			xero::math::PositionAngle getPredictedVelocity();

			void addActualPosition(double t, const xero::math::PositionAngle &measured, const xero::math::PositionAngle &predicted);
			xero::math::PositionAngle generateOdometryFromSensors(double left_dist, double right_dist, const xero::motion::Rotation &rot);

			std::vector<std::pair<double, xero::math::PositionCS>> getPositions()
			{
				std::lock_guard<std::mutex> lock(m_lock);
				std::vector<std::pair<double, xero::math::PositionCS>> copy = m_actual_positions;
				return copy;
			}

		private:
			size_t getBeforeIndex(double t);

		private:
			static RobotState *TheOneObject;
			double m_driven_distance;
			xero::math::PositionAngle m_predicted_velocity;
			double m_time_window;
			std::mutex m_lock;
			std::vector<std::pair<double, xero::math::PositionCS>> m_actual_positions ;
		};
	}
}
