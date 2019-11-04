#pragma once

#include "Position.h"
#include <vector>
#include <cassert>

namespace xero
{
	namespace jacispath
	{
		class Spline
		{
		public:
			Spline();
			~Spline();

			void setStart(const xero::base::Position &c)
			{
				m_offset = c;
			}

			void setKnot(double v)
			{
				m_knot_distance = v;
			}

			double getKnot() const
			{
				return m_knot_distance;
			}

			void setAngle(double v)
			{
				m_angle_offset = v;
			}

			double getAngleOffset() const
			{
				return m_angle_offset;
			}

			xero::base::Position coords(double per);
			double deriv(double per);
			double deriv(const std::vector<double> &coeffs, double k, double p);
			double angle(double per);
			double distance(size_t count);
			double progressForDistance(double dist, size_t count);

			void setCoeffs(const std::vector<double> &c)
			{
				assert(c.size() == 5);
				m_coeffs = c;
			}

		private:
			static constexpr size_t a = 0;
			static constexpr size_t b = 1;
			static constexpr size_t c = 2;
			static constexpr size_t d = 3;
			static constexpr size_t e = 4;

		private:
			std::vector<double> m_coeffs;
			double m_knot_distance;
			double m_angle_offset;
			double m_arc_length;
			xero::base::Position m_offset;
		};
	}
}

