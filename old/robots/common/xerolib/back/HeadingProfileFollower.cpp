#include "HeadingProfileFollower.h"
#include "Rotation.h"

using namespace xero::math;

namespace xero
{
	namespace motion
	{
		double HeadingProfileFollower::update(const State &latest, double t)
		{
			double pos;
			auto set_p = getSetpoint();
			auto goal_p = getGoal();
			Rotation goal_r_inverse = Rotation::fromDegrees(goal_p->getPosition()).inverse();

			if (set_p != nullptr)
			{
				pos = goal_p->getPosition() + goal_r_inverse.rotateBy(Rotation::fromDegrees(set_p->getState().getPosition())).getDegrees();
				State st(set_p->getState().getTime(), pos, set_p->getState().getVelocity(), set_p->getState().getAccel());
				set_p->setState(st);
			}

			pos = goal_p->getPosition() + goal_r_inverse.rotateBy(Rotation::fromDegrees(latest.getPosition())).getDegrees();
			State latest_st_unwrapped(latest.getTime(), pos, latest.getVelocity(), latest.getAccel());

			double result = ProfileFollower::update(latest_st_unwrapped, t);

			if (std::fabs(latest_st_unwrapped.getPosition() - goal_p->getPosition()) < goal_p->getPositionTolerance())
			{
				result = 0.0;
				resetIntegral();
			}

			return result;
		}

		State HeadingProfileFollower::canonicalize(const State &st)
		{
			double pos = Rotation::fromDegrees(st.getPosition()).getDegrees();
			return State(st.getTime(), pos, st.getVelocity(), st.getAccel());
		}
	}
}
