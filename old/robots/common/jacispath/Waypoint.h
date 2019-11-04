#pragma once

namespace xero
{
	namespace jacispath
	{
		class Waypoint
		{
		public:
			Waypoint()
			{
				m_x = 0.0;
				m_y = 0.0;
				m_angle = 0.0;
			}

			Waypoint(double x, double y, double a)
			{
				m_x = x;
				m_y = y;
				m_angle = a;
			}

			double getX() const
			{
				return m_x;
			}

			double getY() const
			{
				return m_y;
			}

			double getAngle() const
			{
				return m_angle;
			}

		private:
			double m_x;
			double m_y;
			double m_angle;
		};
	}
}