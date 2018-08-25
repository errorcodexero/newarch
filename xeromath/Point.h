#pragma once

#include <string>

namespace xero {
	namespace math {
		class Point {
		public:
			Point() ;
			Point(double x, double y) ;

			double getX() const {
				return x_ ;
			}

			double getY() const {
				return y_ ;
			}

			Point translate(double x, double y) const ;
			Point translate(const Point &offset) const ;

			double distance(const Point &other) const ;

			std::string toString() const ;

			double dot(const Point &pt) const ;
			double norm() const ;

			Point scale(double v) const ;
			Point rotate(double angle) const ;

			static double cross(const Point &p1, const Point &p2) ;
			static double angle(const Point &p1, const Point &p2) ;

		private:
			double x_ ;
			double y_ ;
		} ;

		inline Point operator-(const Point &pt) {
			return Point(-pt.getX(), -pt.getY()) ;
		}

		inline Point operator-(const Point &p1, const Point &p2) {
			return Point(p1.getX() - p2.getX(), p1.getY() - p2.getY()) ;
		}

		inline Point operator+(const Point &p1, const Point &p2) {
			return Point(p1.getX() + p2.getX(), p1.getY() + p2.getY()) ;
		}		

		inline Point operator*(double v, const Point &pt) {
			return Point(pt.getX() * v, pt.getY() * v) ;
		}
	}
}
