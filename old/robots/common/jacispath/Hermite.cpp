#include "Hermite.h"
#include "NumUtils.h"
#include <cmath>

using namespace xero::base;

namespace xero
{
	namespace jacispath
	{
		void Hermite::cubic(const xero::jacispath::Waypoint &a,
			const xero::jacispath::Waypoint &b,
			xero::jacispath::Spline &spline)
		{
			fit_pre(a, b, spline);
			double a0d = tan(bound_radians(a.getAngle() - spline.getAngleOffset()));
			double a1d = tan(bound_radians(b.getAngle() - spline.getAngleOffset()));

			double ac = 0.0;
			double bc = 0.0;
			double cc = (a0d + a1d) / (spline.getKnot() * spline.getKnot());
			double dc = -(2 * a0d + a1d) / spline.getKnot();
			double ec = a0d;
			std::vector<double> co{ ac, bc, cc, dc, ec };
			spline.setCoeffs(co);
		}

		void Hermite::quintic(const xero::jacispath::Waypoint &a,
			const xero::jacispath::Waypoint &b,
			xero::jacispath::Spline &spline)
		{
			fit_pre(a, b, spline);
			double a0d = tan(bound_radians(a.getAngle() - spline.getAngleOffset()));
			double a1d = tan(bound_radians(b.getAngle() - spline.getAngleOffset()));

			double dd = spline.getKnot();

			double ac = -(3 * (a0d + a1d)) / (dd * dd * dd * dd);
			double bc = (8 * a0d + 7 * a1d) / (dd * dd * dd);
			double cc = -(6 * a0d + 4 * a1d) / (dd * dd);
			double dc = 0;
			double ec = a0d;

			std::vector<double> co{ ac, bc, cc, dc, ec };
			spline.setCoeffs(co);
		}

		void Hermite::fit_pre(const xero::jacispath::Waypoint &a,
			const xero::jacispath::Waypoint &b,
			xero::jacispath::Spline &spline)
		{
			spline.setStart(Position(a.getX(), a.getY()));
			double dx = b.getX() - a.getX();
			double dy = b.getY() - a.getY();

			double delta = std::sqrt(dx * dx + dy * dy);
			spline.setKnot(delta);
			spline.setAngle(std::atan2(dy, dx));
		}
	}
}
