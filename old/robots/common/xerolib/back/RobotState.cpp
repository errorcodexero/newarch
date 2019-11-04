#include "RobotState.h"
#include "Kinematics.h"
#include <Timer.h>
#include <cassert>
#include <iostream>

using namespace xero::math;
using namespace xero::motion;

namespace xero
{
	namespace pathfinder
	{
		RobotState *RobotState::TheOneObject = nullptr;

		RobotState::RobotState()
		{
			m_driven_distance = 0;
			PositionCS pos;
			reset(frc::Timer::GetFPGATimestamp(), pos);
			m_time_window = 10.0;
		}

		RobotState::~RobotState()
		{
		}

		void RobotState::reset(double start, const PositionCS &pos)
		{
			m_actual_positions.push_back(std::make_pair(start, pos));
		}

		void RobotState::reset(const PositionCS &pos)
		{
			std::lock_guard<std::mutex> lock(m_lock);
			reset(frc::Timer::GetFPGATimestamp(), pos);
		}

		void RobotState::addActualPosition(double t, const xero::math::PositionAngle &measured, const xero::math::PositionAngle &predicted)
		{
			std::lock_guard<std::mutex> lock(m_lock);

			const PositionCS &latest = m_actual_positions[m_actual_positions.size() - 1].second;
			PositionCS ob = Kinematics::integrateForwardKinematics(latest, measured);
			m_actual_positions.push_back(std::make_pair(t, ob));

			while (m_actual_positions.size() > 0 && t - m_actual_positions[0].first > m_time_window)
				m_actual_positions.erase(m_actual_positions.begin());

			m_predicted_velocity = predicted;
		}

		PositionAngle RobotState::generateOdometryFromSensors(double left_dist, double right_dist, const xero::motion::Rotation &rot)
		{
			PositionCS last = getLatestFieldToVehicle();
			PositionAngle delta = Kinematics::forwardKinematics(last.getRotation(), left_dist, right_dist, rot);
			m_driven_distance += delta.getX();

			return delta;
		}

		size_t RobotState::getBeforeIndex(double t)
		{
			if (t < m_actual_positions[0].first)
				return 0;

			size_t left = 0;
			size_t right = m_actual_positions.size() - 1;

			while (right - left > 1)
			{
				size_t mid = (right + left) / 2;
				double sample = m_actual_positions[mid].first;
				if (t < sample)
				{
					right = mid;
				}
				else
				{
					left = mid;
				}
			}

			return left;
		}

		xero::math::PositionCS RobotState::getPositionAtTime(double t)
		{
			std::lock_guard<std::mutex> lock(m_lock);

			assert(m_actual_positions.size() > 0);

			if (m_actual_positions.size() == 1)
				return m_actual_positions[0].second;

			size_t index = getBeforeIndex(t);
			double x = (t - m_actual_positions[index].first) / (m_actual_positions[index + 1].first - m_actual_positions[index].first);
			return m_actual_positions[index].second.interpolate(m_actual_positions[index + 1].second, x);
		}

		double RobotState::getDrivenDistance()
		{
			std::lock_guard<std::mutex> lock(m_lock);
			return m_driven_distance;
		}

		xero::math::PositionAngle RobotState::getPredictedVelocity()
		{
			std::lock_guard<std::mutex> lock(m_lock);
			return m_predicted_velocity;
		}
	}
}
