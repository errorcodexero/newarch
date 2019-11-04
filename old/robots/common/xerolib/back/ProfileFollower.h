#pragma once

#include "State.h"
#include "SetpointGenerator.h"

namespace xero
{
	namespace motion
	{
		class ProfileFollower
		{
		private:
			double m_kp;
			double m_ki;
			double m_kv;
			double m_kffv;
			double m_kffa;

			double m_min_output;
			double m_max_output;

			std::shared_ptr<State> m_latest_state_p;
			std::shared_ptr<State> m_initial_state_p;

			double m_latest_pos_error;
			double m_latest_vel_error;

			double m_total_error;

			std::shared_ptr<ProfileGoal> m_goal_p;
			std::shared_ptr<Constraints> m_constraints_p;
			std::shared_ptr<SetpointGenerator::Setpoint> m_latest_setpoint_p;

			SetpointGenerator m_generator;

		public:
			ProfileFollower(double kp, double ki, double kv, double kffv, double kffa)
			{
				m_min_output = -std::numeric_limits<double>::infinity();
				m_max_output = std::numeric_limits<double>::infinity();

				resetProfile();
				setGains(kp, ki, kv, kffv, kffa);
			}

			virtual ~ProfileFollower()
			{
			}

			void resetProfile()
			{
				m_total_error = 0.0;
				m_latest_state_p = nullptr;
				m_initial_state_p = nullptr;
				m_latest_pos_error = std::nan("");
				m_latest_vel_error = std::nan("");
				m_generator.reset();
				m_goal_p = nullptr;
				m_constraints_p = nullptr;

				resetSetpoint();
			}

			void setGains(double kp, double ki, double kv, double kffv, double kffa)
			{
				m_kp = kp;
				m_ki = ki;
				m_kv = kv;
				m_kffv = kffv;
				m_kffa = kffa;
			}

			void resetSetpoint()
			{
				m_latest_setpoint_p = nullptr;
			}

			void resetIntegral()
			{
				m_total_error = 0;
			}

			void setGoalAndConstraints(const ProfileGoal &goal, const Constraints &con)
			{
				if (m_goal_p != nullptr && *m_goal_p != goal && m_latest_setpoint_p != nullptr)
					m_latest_setpoint_p->isFinal(false);

				setGoal(goal);
				setConstraints(con);
			}

			void setGoal(const ProfileGoal &goal)
			{
				m_goal_p = std::make_shared<ProfileGoal>(goal);
			}

			std::shared_ptr<ProfileGoal> getGoal()
			{
				return m_goal_p;
			}

			void setConstraints(const Constraints &con)
			{
				m_constraints_p = std::make_shared<Constraints>(con);
			}

			std::shared_ptr<SetpointGenerator::Setpoint> getSetpoint()
			{
				return m_latest_setpoint_p;
			}

			void setMinOutput(double v)
			{
				m_min_output = v;
			}

			void setMaxOutput(double v)
			{
				m_max_output = v;
			}

			double getPositionError() const
			{
				return m_latest_pos_error;
			}

			double getVelocityError() const
			{
				return m_latest_vel_error;
			}

			bool isFinishedProfile() const
			{
				return m_goal_p != nullptr && m_latest_setpoint_p != nullptr && m_latest_setpoint_p->isFinal();
			}

			bool onTarget() const;
			virtual double update(const State &latest, double t);

		};
	}
}

