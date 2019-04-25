#include "Spline.h"
#include "NumUtils.h"

using namespace xero::base;

namespace xero
{
	namespace jacispath
	{
		Spline::Spline()
		{
			for(size_t i = 0 ; i < 5 ; i++)
				m_coeffs.push_back(0.0);

			m_angle_offset = 0.0;
			m_arc_length = 0.0;
			m_knot_distance = 0.0;
		}

		Spline::~Spline()
		{
		}

		Position Spline::coords(double per)
		{
			per = clamp(per, 0.0, 1.0);
			double x = per * m_knot_distance;
			double y = (m_coeffs[a] * x + m_coeffs[b]) * (x * x * x * x) + (m_coeffs[c] * x + m_coeffs[d]) * (x * x) + m_coeffs[e] * x;
			double cos_theta = cos(m_angle_offset);
			double sin_theta = sin(m_angle_offset);

			Position c(x * cos_theta - y * sin_theta, x * sin_theta + y * cos_theta);
			return c + m_offset;
		}

		double Spline::deriv(double per)
		{
			double x = per * m_knot_distance;
			return (5 * m_coeffs[a] * x + 4 * m_coeffs[b]) * x * x * x + (3 * m_coeffs[c] * x + 2 * m_coeffs[d]) * x + m_coeffs[e];
		}

		double Spline::deriv(const std::vector<double> &coeffs, double k, double p)
		{
			double x = p * k;
			return (5 * coeffs[a] * x + 4 * coeffs[b]) * x * x * x + (3 * coeffs[c] * x + 2 * coeffs[d]) * x + coeffs[e];
		}

		double Spline::angle(double per)
		{
			return bound_radians(atan(deriv(per))) + m_angle_offset;
		}

		double Spline::distance(size_t count)
		{
			double deriv0 = deriv(m_coeffs, m_knot_distance, 0);
			double last_integrand = std::sqrt(1 + deriv0 * deriv0) / (double)(count);
			double integrand = 0.0;
			double arc_length = 0.0;

			for (size_t i = 0; i <= count; i++)
			{
				double t = i / (double)count;
				double dydt = deriv(m_coeffs, m_knot_distance, t);
				integrand = std::sqrt(1 + dydt * dydt) / (double)count;
				arc_length += (integrand + last_integrand) / 2;
				last_integrand = integrand;
			}

			m_arc_length = m_knot_distance * arc_length;
			return m_arc_length;
		}

		double Spline::progressForDistance(double dist, size_t count)
		{
			double deriv0 = deriv(m_coeffs, m_knot_distance, 0.0);
			double last_integrand = std::sqrt(1 + deriv0 * deriv0) / (double)(count);
			double integrand = 0.0;
			double arc_length = 0.0;
			double last_arc_length = 0.0;
			double t;

			dist /= m_knot_distance;

			for (size_t i = 0; i < count; i++)
			{
				t = i / (double)count;
				double dydt = deriv(m_coeffs, m_knot_distance, t);
				integrand = std::sqrt(1 + dydt * dydt) / (double)count;
				arc_length += (integrand + last_integrand) / 2;
				if (arc_length > dist)
					break;

				last_integrand = integrand;
				last_arc_length = arc_length;
			}

			double interpolated = t;
			if (arc_length != last_arc_length)
				interpolated += ((dist - last_arc_length) / (arc_length - last_arc_length) - 1) / (double)count;

			return interpolated;
		}
	}
}
