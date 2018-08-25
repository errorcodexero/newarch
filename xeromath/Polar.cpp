#include "Polar.h"
#include <cmath>

namespace xero {
	namespace math {
		Polar::Polar(double radius, double angle) {
			radius_ = radius ;
			angle_ = angle ;
		}

		Polar::Polar(const Point &pt) {
			radius_ = std::sqrt(pt.getX() * pt.getX() + pt.getY() * pt.getY()) ;
			angle_ = std::atan2(pt.getY(), pt.getX()) ;
		}

		Polar::~Polar() {
		}

		Point Polar::toCartesian() const {
			double x = radius_ * std::cos(angle_) ;
			double y = radius_ * std::sin(angle_) ;

			return Point(x, y) ;
		}
	}
}