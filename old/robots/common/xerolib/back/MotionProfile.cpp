#include "MotionProfile.h"
#include <cmath>
#include <algorithm>
#include <memory>

namespace xero
{
	namespace motion
	{
		MotionProfile::MotionProfile()
		{
		}

		MotionProfile::MotionProfile(const std::list<Segment> &segments)
		{
			for (const Segment &seg : segments)
				append(seg);
		}

		MotionProfile::MotionProfile(const std::vector<Segment> &segments)
		{
			for (const Segment &seg : segments)
				append(seg);
		}

		MotionProfile::~MotionProfile()
		{
		}

		bool MotionProfile::isValid() const
		{
			if (m_segments.size() > 1)
			{
				for (size_t i = 1; i < m_segments.size(); i++)
				{
					const Segment &prev = m_segments[i - 1];
					const Segment &seg = m_segments[i];

					if (i == 1 && !prev.isValid())
						return false;

					if (!seg.isValid())
						return false;

					if (!seg.getStart().coincident(prev.getEnd()))
						return false;
				}
			}
			else if (m_segments.size() == 1)
			{
				if (!m_segments[0].isValid())
					return false;
			}

			return true;
		}

		std::shared_ptr<State> MotionProfile::stateByTime(double t)
		{
			if (t < getStartTime() && t + kDelta >= getStartTime())
				return std::make_shared<State>(getStartState());

			if (t > getEndTime() && t - kDelta <= getEndTime())
				return std::make_shared<State>(getEndState());

			for (const Segment &seg : m_segments)
			{
				if (seg.containsTime(t))
					return std::make_shared<State>(seg.getStart().extrapolate(t));
			}

			return nullptr;
		}

		State MotionProfile::stateByTimeClamped(double t)
		{
			if (t <= getStartTime())
				return getStartState();
			else if (t >= getEndTime())
				return getEndState();

			for (const Segment &seg : m_segments)
			{
				if (seg.containsTime(t))
					return seg.getStart().extrapolate(t);
			}

			throw std::runtime_error("should not get here");
		}

		std::shared_ptr<State> MotionProfile::firstStateByPosition(double pos)
		{
			for (const Segment &seg : m_segments)
			{
				if (seg.containsPosition(pos))
				{
					if (std::fabs(seg.getEnd().getPosition() - pos) < kDelta)
						return std::make_shared<State>(seg.getEnd());

					double t = std::min(seg.getStart().nextTimeAtPosition(pos), seg.getEnd().getTime());
					if (std::isinf(t))
						return nullptr;

					return std::make_shared<State>(seg.getStart().extrapolate(t));
				}
			}

			return nullptr;
		}

		void MotionProfile::trimBeforeTime(double t)
		{
			while (m_segments.size() > 0 && t >= m_segments[0].getEnd().getTime())
				m_segments.erase(m_segments.begin());

			if (m_segments.size() > 0 && m_segments[0].getEnd().getTime() > t)
				m_segments[0].setStart(m_segments[0].getStart().extrapolate(t));
		}

		void MotionProfile::consolidate()
		{
			size_t i = 0;

			while (m_segments.size() > 1 && i < m_segments.size() - 1)
			{
				const Segment &current = m_segments[i];
				if (current.getStart().coincident(current.getEnd()))
				{
					m_segments.erase(m_segments.begin() + i);
				}
				else
				{
					i++;
				}
			}
		}
	}
}
