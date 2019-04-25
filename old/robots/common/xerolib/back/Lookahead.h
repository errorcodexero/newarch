#pragma once

#include <algorithm>
#include <cmath>

namespace xero
{
	namespace pathfinder
	{
		class Lookahead
		{
		private:
			double m_min_distance;
			double m_max_distance;
			double m_min_speed;
			double m_max_speed;
			double m_delta_distance;
			double m_delta_speed;

			static constexpr double kDelta = 1e-6;

		public:
			Lookahead()
			{
				m_min_distance = 0;
				m_max_distance = 0;
				m_min_speed = 0;
				m_max_speed = 0;
				m_delta_distance = 0;
				m_delta_speed = 0;
			}

			Lookahead(double mindist, double maxdist, double minspeed, double maxspeed)
			{
				m_min_distance = mindist;
				m_max_distance = maxdist;
				m_min_speed = minspeed;
				m_max_speed = maxspeed;

				m_delta_distance = maxdist - mindist;
				m_delta_speed = maxspeed - minspeed;
			}

			virtual ~Lookahead()
			{
			}

			double getMinDistance() const
			{
				return m_min_distance;
			}

			double getMaxDistance() const
			{
				return m_max_distance;
			}

			double getMinSpeed() const
			{
				return m_min_speed;
			}

			double getMaxSpeed() const
			{
				return m_max_speed;
			}

			double getLookaheadForSpeed(double speed) const
			{
				double ret;

				if (std::fabs(m_delta_speed) < kDelta)
				{
					ret = m_min_distance;
				}
				else
				{
					double la = m_delta_distance * (speed - m_min_speed) / m_delta_speed + m_min_distance;
					ret = std::max(m_min_distance, std::min(m_max_distance, la));
				}

				return ret;
			}
		};
	}
}
