#include "Point.h"
#include <cmath>
#include <algorithm>

namespace xero {
    namespace math {
        Point::Point() {
            x_ = 0.0 ;
            y_ = 0.0 ;
        }

        Point::Point(double x, double  y) {
            x_ = x ;
            y_ = y ;
        }

        Point Point::translate(double x, double y) const {
            return Point(x_ + x, y_ + y) ;
        }

        Point Point::translate(const Point &offset) const {
            return Point(x_ + offset.getX(), y_ + offset.getY()) ;
        }

        double Point::distance(const Point &other) const {
            double dx = getX() - other.getX() ;
            double dy = getY() - other.getY() ;

            return std::sqrt(dx * dx + dy * dy) ;               
        }

        double Point::angle(const Point &p1, const Point &p2) {
            double n1 = p1.norm() ;
            double n2 = p2.norm() ;

            if (n1 == 0.0 || n2 == 0.0)
                return 0.0 ;

            double cosangle = p1.dot(p2) / (n1 * n2) ;
            cosangle = std::min(1.0, std::max(cosangle, -1.0)) ;
            return std::acos(cosangle) ;
        }

        double Point::cross(const Point &p1, const Point &p2) {
            return p1.getX() * p2.getY() - p1.getY() * p2.getX() ;
        }

        double Point::dot(const Point &other) const {
            return getX() * other.getX() + getY() * other.getY() ;
        }

        double Point::norm() const {
            return std::sqrt(getX() * getX() + getY() * getY()) ;
        }

        Point Point::scale(double v) const {
            return Point(x_ * v, y_ * v) ;
        }

        Point Point::rotate(double v) const {
            double x = x_ * std::cos(v) - y_ * std::sin(v) ;
            double y = x_ * std::sin(v) + y_ * std::cos(v) ;

            return Point(x, y) ;
        }

        std::string Point::toString() const {
            std::string ret ;

            ret = "[" ;
            ret += "X:" + std::to_string(getX()) ;
            ret += ", Y:" + std::to_string(getY()) ;
            ret += "]" ;
            return ret ;
        }
    }
}
