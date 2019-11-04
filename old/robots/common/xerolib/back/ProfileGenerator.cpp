#include "ProfileGenerator.h"
#include <algorithm>

namespace xero
{
	namespace motion
	{
		std::shared_ptr<MotionProfile> ProfileGenerator::generateProfile(const Constraints &con, const ProfileGoal &goal, const State &curr)
		{
			double delta = goal.getPosition() - curr.getPosition();
			if (delta <= 0.0 || (delta == 0.0 &&  curr.getVelocity() < 0.0))
				return generateFlippedProfile(con, goal, curr);

			double vel = std::copysign(std::min(std::fabs(curr.getVelocity()), con.getMaxVelocity()), curr.getVelocity());
			double acc = std::copysign(std::min(std::fabs(curr.getAccel()), con.getMaxAccel()), curr.getAccel());
			State now(curr.getTime(), curr.getPosition(), vel, acc);

			std::shared_ptr<MotionProfile> profile_p = std::make_shared<MotionProfile>();
			profile_p->reset(now);

			if (now.getVelocity() < 0.0 && delta > 0.0)
			{
				//
				// We are headed away from our goal, must first stop
				//
				double stoptime = std::fabs(curr.getVelocity() / con.getMaxAccel());
				profile_p->append(con.getMaxAccel(), stoptime);
				now = profile_p->getEndState();
				delta = goal.getPosition() - now.getPosition();
			}

			double minvelsq = now.getVelocity() * now.getVelocity() - 2.0 * con.getMaxAccel() * delta;
			double minvel = std::sqrt(std::fabs(minvelsq));
			double maxvel = std::sqrt(now.getVelocity() * now.getVelocity() + 2.0 * con.getMaxAccel() * delta);
			double goalvel = goal.getMaxVelocity();
			double maxacc = con.getMaxAccel();

			if (minvelsq > 0.0 && minvel > (goal.getMaxVelocity() + goal.getVelocityTolerance()))
			{
				//
				// Overshoot is unavoidable, see what we should do
				//
				if (goal.getType() == ProfileGoal::CompletionType::ExceedMaxVelocity)
				{
					//
					// Reset the goal velocity to match what we will hit
					//
					goalvel = minvel;
				}
				else if (goal.getType() == ProfileGoal::CompletionType::ExceedMaxAccel)
				{
					if (std::fabs(delta) < goal.getPositionTolerance())
					{
						//
						// We are at the goal, but moving too fast
						//
						State begin(profile_p->getEndTime(), profile_p->getEndPosition(), profile_p->getEndState().getVelocity(), -std::numeric_limits<double>::infinity());
						State end(profile_p->getEndTime(), profile_p->getEndPosition(), goalvel, -std::numeric_limits<double>::infinity());
						profile_p->append(Segment(begin, end));
						profile_p->consolidate();
						return profile_p;
					}

					maxacc = std::fabs(goalvel * goalvel - now.getVelocity() * now.getVelocity()) / (2.0 * delta);
				}
				else
				{
					//
					// We are going to overshoot the goal, stop then go backwards
					//
					double stoptime = std::fabs(now.getVelocity() / con.getMaxAccel());
					profile_p->append(-con.getMaxAccel(), stoptime);
					profile_p->append(*generateFlippedProfile(con, goal, profile_p->getEndState()));
					profile_p->consolidate();

					return profile_p;
				}
			}

			goalvel = std::min(goalvel, maxvel);

			double vmaxpos = std::sqrt((now.getVelocity() * now.getVelocity() + goalvel * goalvel) / 2.0 + delta * maxacc);
			double vmax = std::min(con.getMaxVelocity(), vmaxpos);

			if (vmax > now.getVelocity())
			{
				//
				// Accelerate to vmax
				//
				double acceltime = (vmax - now.getVelocity()) / maxacc;
				profile_p->append(maxacc, acceltime);
				now = profile_p->getEndState();
			}

			double distdecel = (now.getVelocity() * now.getVelocity() - goalvel * goalvel) / (2.0 * con.getMaxAccel());
			if (distdecel < 0.0)
				distdecel = 0.0;

			double distcruise = (goal.getPosition() - now.getPosition() - distdecel);
			if (distcruise < 0.0)
				distcruise = 0.0;

			if (distcruise > 0.0)
			{
				double cruisetime = distcruise / now.getVelocity();
				profile_p->append(0.0, cruisetime);
				now = profile_p->getEndState();
			}

			if (distdecel > 0.0)
			{
				double deceltime = (now.getVelocity() - goalvel) / maxacc;
				profile_p->append(-maxacc, deceltime);
			}

			profile_p->consolidate();
			return profile_p;
		}

		std::shared_ptr<MotionProfile> ProfileGenerator::generateFlippedProfile(const Constraints &con, const ProfileGoal &goal, const State &curr)
		{
			auto ptr = generateProfile(con, goal.flipped(), curr.flipped());
			for (Segment &seg : ptr->getSegments())
			{
				seg.setStart(seg.getStart().flipped());
				seg.setEnd(seg.getEnd().flipped());
			}

			return ptr;
		}
	}
}
