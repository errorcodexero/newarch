#pragma once
#include <string>
#include <cmath>

namespace xero
{
	namespace motion
	{
		class State
		{
		private:
			static constexpr double kDelta = 1.0e-9;

		private:
			double m_time;
			double m_position;
			double m_velocity;
			double m_accel;

		public:
			State();
			State(double t, double pos, double vel, double accel);
			virtual ~State();

			double getTime() const
			{
				return m_time;
			}

			double getPosition() const
			{
				return m_position;
			}

			double getVelocity() const
			{
				return m_velocity;
			}

			double getAccel() const
			{
				return m_accel;
			}

			State extrapolate(double t) const
			{
				return extrapolate(t, m_accel);
			}

			State extrapolate(double t, double accel) const
			{
				double dt = t - m_time;
				double pos = m_position + m_velocity * dt + 0.5 * accel * dt * dt;
				double vel = m_velocity + accel * dt;
				return State(t, pos, vel, accel);
			}

			double nextTimeAtPosition(double pos) const;

			bool coincident(const State &st) const;

			State flipped() const
			{
				State st(m_time, -m_position, -m_velocity, -m_accel);
				return st;
			}

			std::string toString() const
			{
				std::string ret("State: ");
				ret += "time " + std::to_string(m_time);
				ret += ", pos " + std::to_string(m_position);
				ret += ", vel " + std::to_string(m_velocity);
				ret += ", acc " + std::to_string(m_accel);

				return ret;
			}

			bool operator==(const State &st) const
			{
				if (std::fabs(m_time - st.getTime()) > kDelta)
					return false;

				if (std::fabs(m_position - st.getPosition()) > kDelta)
					return false;

				if (std::fabs(m_velocity - st.getVelocity()) > kDelta)
					return false;

				return true;
			}

			bool operator!=(const State &st) const
			{
				return !(*this == st);
			}

		protected:
			static bool signIsSame(double a, double b)
			{
				return (a <= 0 && b <= 0) || (a >= 0 && b >= 0);
			}
		};
	}
}

