#pragma once

#include "Position.h"

namespace xero
{
	namespace motion
	{
		class Segment
		{
		public:
			Segment()
			{
				m_dt = 0.0;
				m_position = 0.0;
				m_velocity = 0.0;
				m_acceleration = 0.0;
				m_jerk = 0.0;
				m_heading = 0.0;
			}

			Segment(double dt, double x, double y, double pos, double vel, double accel, double jerk, double head)
			{
				m_dt = dt;
				m_coords = xero::base::Position(x, y);
				m_position = pos;
				m_velocity = vel;
				m_acceleration = accel;
				m_jerk = jerk;
				m_heading = head;
			}

			void setDT(double t)
			{
				m_dt = t;
			}

			void setHeading(double angle)
			{
				m_heading = angle;
			}

			void setCoords(const xero::base::Position &pos)
			{
				m_coords = pos;
			}

			void setCoords(double x, double y)
			{
				m_coords = xero::base::Position(x, y);
			}

			void setPosition(double p)
			{
				m_position = p;
			}

			void setAcceleration(double a)
			{
				m_acceleration = a;
			}

			void setVelocity(double v)
			{
				m_velocity = v;
			}

			void setJerk(double j)
			{
				m_jerk = j;
			}

			double getDT() const
			{
				return m_dt;
			}

			const xero::base::Position &getCoords() const
			{
				return m_coords;
			}

			double getX() const
			{
				return m_coords.getX();
			}

			double getY() const
			{
				return m_coords.getY();
			}

			double getPosition() const
			{
				return m_position;
			}

			double getVelocity() const
			{
				return m_velocity;
			}

			double getAcceleration() const
			{
				return m_acceleration;
			}

			double getJerk() const
			{
				return m_jerk;
			}

			double getHeading() const
			{
				return m_heading;
			}

		private:
			double m_dt;
			xero::base::Position m_coords;
			double m_position;
			double m_velocity;
			double m_acceleration;
			double m_jerk;
			double m_heading;
		};
	}
}