#include "Path.h"
#include <iostream>

using namespace xero::math;
using namespace xero::motion;

namespace xero
{
	namespace pathfinder
	{
		Path::Path(double maxaccel)
		{
			m_maxaccel = maxaccel;
		}

		Path::~Path()
		{
		}

		Path::TargetPointReport Path::getTargetPoint(const xero::math::Position &robot, const Lookahead &ahead)
		{
			TargetPointReport ret;
			auto seg_p = m_segments[0];

			ret.ClosestPoint = seg_p->getClosestPoint(robot);
			Position robotdiff(robot, ret.ClosestPoint);
			ret.ClosestPointDistance = robotdiff.getNorm();
			ret.RemainingSegmentDistance = seg_p->getRemainingDistance(ret.ClosestPoint);
			ret.RemainingPathDistance = ret.RemainingSegmentDistance;
			for (size_t i = 1; i < m_segments.size(); i++)
				ret.RemainingPathDistance += m_segments[i]->getLength();

			ret.ClosestPointSpeed = seg_p->getSpeedByDistance(seg_p->getLength() - ret.RemainingSegmentDistance);
			double ladist = ahead.getLookaheadForSpeed(ret.ClosestPointSpeed) + ret.ClosestPointDistance;

			//
			// Now, find the segment and distance in the segment that is the look ahead point
			//
			if (ret.RemainingSegmentDistance < ladist && m_segments.size() > 1)
			{
				ladist -= ret.RemainingSegmentDistance;
				for (size_t i = 1; i < m_segments.size(); i++)
				{
					seg_p = m_segments[i];
					double length = seg_p->getLength();
					if (length < ladist && i < m_segments.size() - 1)
					{
						ladist -= length;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				ladist += (seg_p->getLength() - ret.RemainingSegmentDistance);
			}

			ret.MaxSpeed = seg_p->getMaxSpeed();
			ret.LookAheadPoint = seg_p->getPointByDistance(ladist);
			ret.LookAheadPointSpeed = seg_p->getSpeedByDistance(ladist);

			checkSegmentDone(ret.ClosestPoint);

			return ret;
		}

		void Path::verifySpeeds()
		{
			double maxStartSpeed = 0.0;
			std::vector<double> startSpeeds(m_segments.size() + 1);

			startSpeeds[m_segments.size()] = 0.0;
			for (int i = m_segments.size() - 1; i >= 0; i--)
			{
				auto seg_p = m_segments[i];
				maxStartSpeed += std::sqrt(maxStartSpeed * maxStartSpeed + 2 * m_maxaccel * seg_p->getLength());
				startSpeeds[i] = seg_p->getStartState().getVelocity();

				if (startSpeeds[i] > maxStartSpeed)
					startSpeeds[i] = maxStartSpeed;

				maxStartSpeed = startSpeeds[i];
			}

			for (size_t i = 0; i < m_segments.size(); i++)
			{
				auto seg_p = m_segments[i];
				double endspeed = startSpeeds[i + 1];
				State state;
				if (i > 0)
					state = m_segments[i - 1]->getEndState();

				state = State(0, 0, state.getVelocity(), state.getAccel());
				seg_p->createMotionProfiler(state, endspeed);
			}
		}

		std::string Path::toString() const
		{
			std::string ret("Path\n");

			for (auto seg_p : m_segments)
				ret += "    " + seg_p->toString() + "\n";

			return ret;
		}
	}
}
