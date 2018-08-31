#pragma once

#include "Point.h"

namespace xero {
	namespace math {

		class Polar {
		public:
			Polar(double radius, double angle) ;
			Polar(const Point &pt) ;
			~Polar() ;

			Point toCartesian() const ;

			Polar rotate(double angle) {
				return Polar(radius_, angle_ + angle) ;
			}

			double getAngle() const {
				return angle_ ;
			}

			double getRadius() const {
				return radius_ ;
			}

		private:
			double radius_ ;
			double angle_ ;
		} ;
	}
}