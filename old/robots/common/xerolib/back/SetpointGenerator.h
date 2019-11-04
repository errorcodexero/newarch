#pragma once

#include "State.h"
#include "MotionProfile.h"
#include "ProfileGoal.h"
#include "Constraints.h"
#include <memory>

namespace xero
{
	namespace motion
	{
		class SetpointGenerator
		{
		public:
			class Setpoint
			{
			private:
				State m_state;
				bool m_final;

			public:
				Setpoint()
				{
					m_final = false;
				}

				Setpoint(State st, bool finalset)
				{
					m_state = st;
					m_final = finalset;
				}

				virtual ~Setpoint()
				{
				}

				void setState(const State &st)
				{
					m_state = st;
				}

				const State &getState() const
				{
					return m_state;
				}

				bool isFinal() const
				{
					return m_final;
				}

				void isFinal(bool v)
				{
					m_final = v;
				}
			};

		private:
			std::shared_ptr<MotionProfile> m_profile_p;
			std::shared_ptr<ProfileGoal> m_goal_p;
			std::shared_ptr<Constraints> m_constraints_p;

		public:
			SetpointGenerator()
			{
				reset();
			}

			virtual ~SetpointGenerator()
			{
			}

			void reset()
			{
				m_profile_p = nullptr;
				m_goal_p = nullptr;
				m_constraints_p = nullptr;
			}

			Setpoint getSetpoint(const Constraints &con, const ProfileGoal &goal, const State &prev, double t);

		private:
			bool doWeRegen(const Constraints &con, const ProfileGoal &goal);
		};
	}
}

