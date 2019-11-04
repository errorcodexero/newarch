#include "ProfileFollower.h"
#include <algorithm>

namespace xero
{
	namespace motion
	{
		bool ProfileFollower::onTarget() const
		{
			if (m_goal_p == nullptr || m_latest_setpoint_p == nullptr)
				return false;

			double goal_to_start = m_goal_p->getPosition() - m_initial_state_p->getPosition();
			double goal_to_actual = m_goal_p->getPosition() - m_latest_state_p->getPosition();

			bool passed = (goal_to_start * goal_to_actual) < 0.0;

			if (m_goal_p->getType() != ProfileGoal::Overshoot && passed)
				return true;

			return m_goal_p->atGoalState(*m_latest_state_p);
		}

		double ProfileFollower::update(const State &latest, double t)
		{
			m_latest_state_p = std::make_shared<State>(latest);
			State prev = latest;

			if (m_latest_setpoint_p != nullptr)
			{
				prev = m_latest_setpoint_p->getState();
			}
			else
			{
				m_initial_state_p = std::make_shared<State>(latest);
			}

			double dt = std::max(0.0, t - prev.getTime());
			m_latest_setpoint_p = std::make_shared<SetpointGenerator::Setpoint>(m_generator.getSetpoint(*m_constraints_p, *m_goal_p, prev, t));

			m_latest_pos_error = m_latest_setpoint_p->getState().getPosition() - latest.getPosition();
			m_latest_vel_error = m_latest_setpoint_p->getState().getVelocity() - latest.getVelocity();

			double output = 0.0;
			
			output += m_kp * m_latest_pos_error;
			output += m_kv * m_latest_vel_error;
			output += m_kffv * m_latest_setpoint_p->getState().getVelocity();

			if (!std::isnan(m_latest_setpoint_p->getState().getAccel()))
				output += m_kffa * m_latest_setpoint_p->getState().getAccel();

			if (output >= m_min_output && output <= m_max_output)
			{
				m_total_error += m_latest_pos_error * dt;
				output += m_ki * m_total_error;
			}
			else
			{
				m_total_error = 0.0;
			}

			output = std::max(m_min_output, std::min(m_max_output, output));

			return output;
		}
	}
}
