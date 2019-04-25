#pragma once

#include "Waypoint.h"
#include "Spline.h"

namespace xero
{
	namespace jacispath
	{

		class Hermite
		{
		public:
			Hermite() = delete;
			~Hermite() = delete;

			static void cubic(const xero::jacispath::Waypoint &a,
				const xero::jacispath::Waypoint &b,
				xero::jacispath::Spline &spline);
			static void quintic(const xero::jacispath::Waypoint &a,
				const xero::jacispath::Waypoint &b,
				xero::jacispath::Spline &spline);

		private:
			static void fit_pre(const xero::jacispath::Waypoint &a,
				const xero::jacispath::Waypoint &b,
				xero::jacispath::Spline &spline);
		};
	}
}

