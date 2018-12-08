#pragma once

#include <string>

/// \file

namespace xero {
	namespace math {
		/// \brief This class represents a point in two dimensional space
		class Point {
		public:
			/// \brief Create a point at the origin
			Point() ;

			/// \brief Create a point at the given x and y coordinates
			/// \param x the x coordinate of the point
			/// \param y the y coordinate of the point
			Point(double x, double y) ;

			/// \brief return the x coordinate of the point
			/// \returns x coordinate for the point
			double getX() const {
				return x_ ;
			}

			/// \brief return the y coordinate of the point
			/// \returns y coordinate for the point
			double getY() const {
				return y_ ;
			}

			/// \brief return a point that is the current point translated by the given x and y distances
			/// \param x the distance to translate the point in the x direction
			/// \param y the distance to translate the point in the y direction
			/// \returns the new point translated by the distance given
			Point translate(double x, double y) const ;

			/// \brief return a point that is the current point translated by the given distances
			/// \param offset a point containing the x and y distances to translate by
			/// \returns the new point translated by the distance given
			Point translate(const Point &offset) const ;

			/// \brief return the distance between the current point and another point
			/// \param other the point in question for the distance calculation
			/// \returns the distance between the current point and the other point
			double distance(const Point &other) const ;

			/// \brief return a human readable string representing the point
			/// \return a string representing the point
			std::string toString() const ;

			/// \brief return the dot product for the point
			/// <br>The equestion is this.x * pt.x + this.y * pt.y
			/// \param other the other point for the dot product
			/// \returns the dot product between thispoint and the other point
			double dot(const Point &other) const ;

			/// \brief returns the distance between the origin an this point
			/// \return the distance between the origin and this point
			double norm() const ;

			/// \brief returns a new point which is the current point scaled by a factor
			/// <br>returned.x = this.x * factor
			/// <br>returned.y = this.y * factor
			/// \param factor the scale factor for the new point
			/// \returns new point scaled by the factor given
			Point scale(double factor) const ;

			/// \brief returns a new point which is the current point rotated about the origin by the angle given
			/// \param angle the angle of rotation in degrees
			/// \returns new point rotated about the origin by the angle given
			Point rotate(double angle) const ;

			/// \brief returns the cross product of the two points given
			/// <br>cross = p1.x * p2.y - p1.y * p2.x
			/// \param p1 the first point
			/// \param p2 the second point
			/// \returns the cross product value
			static double cross(const Point &p1, const Point &p2) ;

			/// \brief returns the angle between two vectors drawn from the origin to each of the points
			/// \param p1 the first point
			/// \param p2 the second point
			/// \returns the angle between the two points
			static double angle(const Point &p1, const Point &p2) ;

		private:
			double x_ ;
			double y_ ;
		} ;

		/// \brief negate a single point
		/// \param pt the point to negate
		/// \returns the point negated
		inline Point operator-(const Point &pt) {
			return Point(-pt.getX(), -pt.getY()) ;
		}

		/// \brief subtract two points returns the different
		/// \param p1 the first point
		/// \param p2 the second point
		/// \returns the difference in the two points
		inline Point operator-(const Point &p1, const Point &p2) {
			return Point(p1.getX() - p2.getX(), p1.getY() - p2.getY()) ;
		}

		/// \brief adds two points returns the different
		/// \param p1 the first point
		/// \param p2 the second point
		/// \returns the sum of the two points
		inline Point operator+(const Point &p1, const Point &p2) {
			return Point(p1.getX() + p2.getX(), p1.getY() + p2.getY()) ;
		}		

		/// \brief multiplies the point by the factor and returns result
		/// \param factor the factor to multiply by
		/// \param pt the point to multiply
		/// \returns the point scaled by the factor
		inline Point operator*(double factor, const Point &pt) {
			return Point(pt.getX() * factor, pt.getY() * factor) ;
		}
	}
}
