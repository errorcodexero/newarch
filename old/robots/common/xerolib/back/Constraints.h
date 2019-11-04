#pragma once

#include <limits>

namespace xero
{
	namespace motion
	{
		class Constraints
		{
		private:
			static constexpr double kDelta = 1.0e-9;

		private:
			double m_max_abs_velocity;
			double m_max_abs_accel;

		public:
			Constraints()
			{
				m_max_abs_velocity = std::numeric_limits<double>::infinity();
				m_max_abs_accel = std::numeric_limits<double>::infinity();
			}

			Constraints(double vel, double accel)
			{
				m_max_abs_velocity = vel;
				m_max_abs_accel = accel;
			}

			virtual ~Constraints()
			{
			}

			double getMaxVelocity() const
			{
				return m_max_abs_velocity;
			}

			double getMaxAccel() const
			{
				return m_max_abs_accel;
			}

			bool operator==(const Constraints &con) const
			{
				if (std::fabs(m_max_abs_velocity - con.getMaxVelocity()) > kDelta)
					return false;

				if (std::fabs(m_max_abs_accel - con.getMaxAccel()) > kDelta)
					return false;

				return true;
			}

			bool operator!=(const Constraints &con) const
			{
				return !(*this == con);
			}
		};
	}
}
