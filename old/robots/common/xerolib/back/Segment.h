#pragma once

#include "State.h"
#include <cmath>

namespace xero
{
	namespace motion
	{
		class Segment
		{
		private:
			static constexpr double kDelta = 1.0e-9;

		private:
			State m_start;
			State m_end;

		public:
			Segment(const State &start, const State &end)
			{
				m_start = start;
				m_end = end;
			}
			virtual ~Segment();

			double length() const
			{
				return std::fabs(m_end.getPosition() - m_start.getPosition());
			}

			const State &getStart() const
			{
				return m_start;
			}

			void setStart(const State &v)
			{
				m_start = v;
			}

			const State &getEnd() const
			{
				return m_end;
			}
			
			void setEnd(const State &v)
			{
				m_end = v;
			}

			bool isValid() const;

			bool containsTime(double t) const
			{
				return t >= m_start.getTime() && t <= m_end.getTime();
			}

			bool containsPosition(double pos) const
			{
				return (pos >= m_start.getPosition() && pos <= m_end.getPosition()) ||
					(pos <= m_start.getPosition() && pos >= m_end.getPosition());
			}

			std::string toString() const
			{
				return "Start: " + m_start.toString() + ", End: " + m_end.toString();
			}
		};
	}
}

