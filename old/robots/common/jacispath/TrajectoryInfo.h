#pragma once

#include "TrajectoryConfig.h"

namespace xero
{
	namespace jacispath
	{
		class TrajectoryInfo
		{
		public:
			TrajectoryInfo(const TrajectoryConfig &c);
			TrajectoryInfo();
			virtual ~TrajectoryInfo();

			int GetFilter1() const
			{
				return m_filter_1;
			}

			int GetFilter2() const
			{
				return m_filter_2;
			}

			int getLength() const
			{
				return m_length;
			}

			double getDT() const
			{
				return m_dt;
			}

			double getU() const
			{
				return m_u;
			}

			double getV() const
			{
				return m_v;
			}

			double getImpulse() const
			{
				return m_impulse;
			}

		private:
			int m_filter_1;
			int m_filter_2;
			int m_length;
			double m_dt;
			double m_u;
			double m_v;
			double m_impulse;
		};
	}
}

