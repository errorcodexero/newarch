#include "State.h"
#include <cmath>
#include <limits>

namespace xero
{
	namespace motion
	{
		State::State()
		{
			m_time = 0;
			m_position = 0;
			m_velocity = 0;
			m_accel = 0;
		}

		State::State(double t, double pos, double vel, double acc)
		{
			m_time = t;
			m_position = pos;
			m_velocity = vel;
			m_accel = acc;
		}

		State::~State()
		{
		}

		double State::nextTimeAtPosition(double pos) const
		{
			double delta = pos - m_position;

			if (std::fabs(pos - m_position) < kDelta)
			{
				//
				// We are already there
				//
				return m_time;
			}
			else if (std::fabs(m_accel) < kDelta)
			{
				//
				// We are not accelerating, simple velocity case
				//
				if (std::fabs(m_velocity) > kDelta && signIsSame(delta, m_velocity))
				{
					//
					// We are heading in the right direction
					//
					return delta / m_velocity + m_time;
				}

				//
				// Velocity is either zero or heading the wrong way.  Fall out of
				// if statement and return inifinity
				//
			}
			else
			{
				//
				// Case with velocity and acceleration
				//
				double d = m_velocity * m_velocity + 2.0 * m_accel * delta;
				if (d < 0.0)
					return std::nan("");

				double sq = std::sqrt(d);

				//
				// Two answers from the quadratic
				//
				double d1 = (-m_velocity + sq) / m_accel;
				double d2 = (-m_velocity - sq) / m_accel;

				//
				// Pick the right answer from the quadratic
				//
				if (d1 >= 0.0 && (d2 < 0 || d1 < d2))
					return m_time + d1;

				if (d2 >= 0.0)
					return m_time + d2;

				//
				// The only answer is in the past, fall through
				// and return infinity
				//
			}
			return std::nan("");
		}

		bool State::coincident(const State &st) const
		{
			if (std::fabs(m_time - st.getTime()) > kDelta)
				return false;

			if (std::fabs(m_position - st.getPosition()) > kDelta)
				return false;

			if (std::fabs(m_velocity - st.getVelocity()) > kDelta)
				return false;

			return true;
		}
	}
}
