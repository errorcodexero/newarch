#pragma once

#include "Point.h"

namespace xero {
	namespace math {
		class PointAngle : public Point {
		public:
			PointAngle() ;
			PointAngle(double x, double y);
			PointAngle(double x, double y, double angle) ;
			PointAngle(const Point &pt, double angle) ;
			~PointAngle() ;

			bool isVertical() const ;
			bool isHorizontal() const ;

			double getAngle() const {
				return angle_ ;
			}

			PointAngle forward(double dist) const ;
			PointAngle translate(double x, double y) const ;
			PointAngle translate(const Point &offset) const ;
			PointAngle rotate(double angle) const ;
			PointAngle perpendicular() const ;
			static Point intersect(const PointAngle &p1, const PointAngle &p2) ;

			std::string toString() const ;

		private:
			double static constexpr AngleTolerance = 1e-8 ;
			double static constexpr SlopeEqualTolerance = 1e-6 ;

		private:
			//
			// The angle, always in radians
			//
			double angle_ ;
		} ;
	}
}