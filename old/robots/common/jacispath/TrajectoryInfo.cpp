#include "TrajectoryInfo.h"
#include <cmath>
#include <algorithm>

namespace xero
{
	namespace jacispath
	{
		TrajectoryInfo::TrajectoryInfo()
		{
			m_filter_1 = 0;
			m_filter_2 = 0;
			m_impulse = 0.0;
			m_length = 0;
			m_dt = 0.0;
			m_u = 0.0;
			m_v = 0.0;
		}

		TrajectoryInfo::TrajectoryInfo(const TrajectoryConfig &c)
		{
			double max_a_sq = c.getMaxAccel() * c.getMaxAccel();
			double max_j_sq = c.getMaxJerk() * c.getMaxJerk();

			double nem = (-max_a_sq + std::sqrt(max_a_sq * max_a_sq + 4.0 * max_j_sq * c.getMaxAccel() * c.getDestPos()));
			double den = 2.0 * c.getMaxJerk();
			double checked_max_v = std::min(c.getMaxVel(), nem / den);

			m_filter_1 = static_cast<int>(std::ceil((checked_max_v / c.getMaxAccel()) / c.getDeltaTime()));
			m_filter_2 = static_cast<int>(std::ceil((c.getMaxAccel() / c.getMaxJerk()) / c.getDeltaTime()));

			m_impulse = (c.getDestPos() / checked_max_v) / c.getDeltaTime();
			m_length = static_cast<int>(std::ceil(m_filter_1 + m_filter_2 + m_impulse));
			m_dt = c.getDeltaTime();
			m_u = 0.0;
			m_v = checked_max_v;
		}

		TrajectoryInfo::~TrajectoryInfo()
		{
		}
	}
}
