#include "PointAngle.h"
#include <xeromath.h>
#include <cmath>
#include <cassert>

namespace xero {
	namespace math {
		PointAngle::PointAngle() {
			angle_ = 0.0 ;
		}

		PointAngle::PointAngle(double x, double y) : Point(x, y) {			
			angle_ = 0.0 ;
		}

		PointAngle::PointAngle(double x, double y , double angle) : Point(x, y) {
			angle_ = angle ;
		}

		PointAngle::PointAngle(const Point &pt, double angle) : Point(pt) {
			angle_ = angle ;
		}

		PointAngle::~PointAngle() {			
		}

		bool PointAngle::isVertical() const {
			return (std::fabs(angle_) - PI/2) < AngleTolerance ;
		}

		bool PointAngle::isHorizontal() const {
			return (std::fabs(angle_)) < AngleTolerance || (std::fabs(angle_) - PI) < AngleTolerance ;
		}

		PointAngle PointAngle::forward(double dist) const {
			double x = getX() + dist * std::cos(angle_) ;
			double y = getY() + dist * std::sin(angle_) ;

			return PointAngle(x, y, angle_) ;
		}

		PointAngle PointAngle::translate(double x, double y) const {
			return PointAngle(getX() + x, getY() + y, angle_) ;
		}

		PointAngle PointAngle::translate(const Point &pt) const {
			return PointAngle(getX() + pt.getX(), getY() + pt.getY(), angle_) ;
		}

		PointAngle PointAngle::rotate(double ang) const {
			return PointAngle(getX(), getY(), xero::math::normalizeAngleRadians(angle_ + ang)) ;
		}

		PointAngle PointAngle::perpendicular() const {
			return rotate(xero::math::PI / 2) ;
		}

		Point PointAngle::intersect(const PointAngle &p1, const PointAngle &p2) {
			if (p1.isVertical() && p2.isVertical())
				return Point(nan("nointersect"), nan("nointersect")) ;

			if (p1.isVertical())
				return intersect(p2, p1) ;

			//
			// At this point we know the slopes are not equal and if there
			// is a vertical point, it is p2
			//
			double x, y ;
			double m1 = std::tan(p1.getAngle()) ;
			double b1 = p1.getY() - m1 * p1.getX() ;

			if (p2.isVertical()) {
				x = p2.getX() ;
			}	
			else {
				double m1 = std::tan(p1.getAngle()) ;
				double m2 = std::tan(p2.getAngle()) ;

				if (std::fabs(m1 - m2) < SlopeEqualTolerance)
					return Point(nan("nointersect"), nan("nointersect")) ;

				double b2 = p2.getY() - m2 * p2.getX() ;
				x = (b2 - b1) / (m1 - m2) ;
			}

			y = m1 * x + b1 ;
			return Point(x, y) ;
		}

		std::string PointAngle::toString() const {
			std::string ret ;

			ret = "[" ;
			ret += "X:" + std::to_string(getX()) ;
			ret += ", Y:" + std::to_string(getY()) ;
			ret += ", A:" + std::to_string(getAngle()) ;
			ret += "]" ;
			return ret ;
		}
	}
}
