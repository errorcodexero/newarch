#pragma once

#include <cmath>
#include <limits>

namespace xero
{
	namespace base
	{
		class Position;

		class Rotation
		{
		private:
			static constexpr double kDelta = 1.0e-9;
			static constexpr double kPI = 3.14159265359;

		private:
			double m_cos;
			double m_sin;

		public:
			Rotation() : Rotation(1, 0, false)
			{
			}

			Rotation(double x, double y, bool norm = true)
			{
				m_cos = x;
				m_sin = y;
				if (norm)
					normalize();
			}

			Rotation(const Position &pos);

			virtual ~Rotation()
			{
			}
			
			static double r2d(double ang)
			{
				return ang / kPI * 180.0;
			}

			static double d2r(double ang)
			{
				return ang / 180.0 * kPI;
			}

			double getCos() const
			{
				return m_cos;
			}

			double getSin() const
			{
				return m_sin;
			}

			double getTan() const
			{
				double ret;

				if (std::fabs(m_cos) < kDelta)
				{
					if (m_sin >= 0)
						ret = std::numeric_limits<double>::infinity();
					else
						ret = -std::numeric_limits<double>::infinity();
				}
				else
				{
					ret = m_sin / m_cos;
				}

				return ret;
			}

			double getRadians() const
			{
				return std::atan2(m_sin, m_cos);
			}

			double getDegrees() const
			{
				return getRadians() * 180.0 / kPI;
			}

			Rotation rotateBy(const Rotation &r) const
			{
				return Rotation(m_cos * r.getCos() - m_sin * r.getSin(), m_cos * r.getSin() + m_sin * r.getCos());
			}

			Rotation normal() const
			{
				return Rotation(-m_sin, m_cos, false);
			}

			Rotation inverse() const
			{
				return Rotation(m_cos, -m_sin, false);
			}

			bool isParallel(const Rotation &r) const
			{
				double cross = m_cos * r.getSin() - m_sin * r.getCos();
				return std::fabs(cross) < kDelta;
			}

			Rotation interpolate(const Rotation &r, double x) const
			{
				Rotation ret;

				if (x <= 0)
				{
					ret = *this;
				}
				else if (x >= 1)
				{
					ret = r;
				}
				else
				{
					double ad = inverse().rotateBy(r).getRadians();
					return rotateBy(Rotation::fromRadians(ad * x));
				}
				return ret;
			}

			static Rotation fromRadians(double rad)
			{
				return Rotation(std::cos(rad), std::sin(rad), false);
			}

			static Rotation fromDegrees(double deg)
			{
				return fromRadians(deg * kPI / 180.0);
			}

		protected:
			void normalize()
			{
				double hyp = std::sqrt(m_cos * m_cos + m_sin * m_sin);
				if (hyp > kDelta)
				{
					m_sin = m_sin / hyp;
					m_cos = m_cos / hyp;
				}
				else
				{
					m_cos = 1.0;
					m_sin = 0.0;
				}
			}
		};
	}
}
