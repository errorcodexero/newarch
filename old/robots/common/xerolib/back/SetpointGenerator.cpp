#include "SetpointGenerator.h"
#include "ProfileGenerator.h"
#include <algorithm>

namespace xero
{
	namespace motion
	{
		bool SetpointGenerator::doWeRegen(const Constraints &con, const ProfileGoal &goal)
		{
			bool regen = false;

			if (m_profile_p == nullptr)
				regen = true;

			if (!regen && (m_constraints_p == nullptr || con != *m_constraints_p))
				regen = true;

			if (!regen && (m_goal_p == nullptr || goal != *m_goal_p))
				regen = true;

			return regen;
		}

		SetpointGenerator::Setpoint SetpointGenerator::getSetpoint(const Constraints &con, const ProfileGoal &goal, const State &prev, double t)
		{
			bool regen = doWeRegen(con, goal);

			if (!regen && !m_profile_p->isEmpty())
			{
				std::shared_ptr<State> state_p = m_profile_p->stateByTime(prev.getTime());
				if (state_p == nullptr || *state_p != prev)
					regen = true;
			}

			if (regen)
			{
				m_constraints_p = std::make_shared<Constraints>(con);
				m_goal_p = std::make_shared<ProfileGoal>(goal);
				m_profile_p = ProfileGenerator::generateProfile(con, goal, prev);
			}

			Setpoint rv;
			bool setvalid = false;

			if (m_profile_p != nullptr && !m_profile_p->isEmpty() && m_profile_p->isValid())
			{
				State st;

				if (t > m_profile_p->getEndTime())
					st = m_profile_p->getEndState();
				else if (t < m_profile_p->getStartTime())
					st = m_profile_p->getStartState();
				else
				{
					auto pt_p = m_profile_p->stateByTime(t);
					if (pt_p == nullptr)
						st = prev;
					else
						st = *pt_p;
				}

				setvalid = true;
				m_profile_p->trimBeforeTime(t);
				rv = Setpoint(st, m_profile_p->isEmpty());
			}

			if (!setvalid)
				rv = Setpoint(prev, true);

			if (rv.isFinal())
			{
				double vel = std::copysign(std::max(goal.getMaxVelocity(), std::fabs(rv.getState().getVelocity())), rv.getState().getVelocity());
				State st(rv.getState().getTime(), goal.getPosition(), vel, 0.0);
				rv.setState(st);
			}

			return rv;
		}
	}
}
