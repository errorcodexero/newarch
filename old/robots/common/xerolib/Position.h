#pragma once

#include <cmath>
#include <string>

namespace xero
{
	namespace base
	{
		class Rotation;

		class Position
		{
		private:
			double m_x;
			double m_y;

		public:
			/// \brief create a position object at the origin
			Position()
			{
				m_x = 0.0;
				m_y = 0.0;
			}

			/// \brief create a new position object with the given coordaintes
			/// \param x the x position of the object
			/// \param y the y position of the object
			Position(double x, double y)
			{
				m_x = x;
				m_y = y;
			}

			/// \brief create a position object that is the difference in two other objects
			/// \param p1 the first object
			/// \param p2 the second object
			Position(const Position &p1, const Position &p2)
			{
				m_x = p2.getX() - p1.getX();
				m_y = p2.getY() - p1.getY();
			}

			/// \brief destroy the position object
			virtual ~Position()
			{
			}

			/// \brief get the X coordinate for the object
			/// \returns the X coordinate for the object
			double getX() const
			{
				return m_x;
			}

			/// \brief get the Y coordinate for the object
			/// \returns the Y coordainte for the object
			double getY() const
			{
				return m_y;
			}

			/// \brief set the X coordinate for the object
			/// \param v the value for the x coordinate
			void setX(double v)
			{
				m_x = v;
			}

			/// \brief set the Y coordinate for the object
			/// \param v the value for the y coordinate
			void setY(double v)
			{
				m_y = v;
			}

			/// \brief return the euclidean distance between the origin and this point
			/// \returns the normalization of the point
			double getNorm() const
			{
				return std::sqrt(m_x * m_x + m_y * m_y);
			}

			/// \brief return the square of the euclidean distance between the origin and this point
			/// \returns the square of the euclidean distance between the origin and this point
			double getNormSquared() const
			{
				return m_x * m_x + m_y * m_y;
			}

			/// \brief return the inverse transform for this object as a transform
			/// \returns the object that is the reverse of this object as a transform
			Position inverse() const
			{
				return Position(-m_x, -m_y);
			}

			/// \brief scale the current point by a factor
			/// \returns a new scaled point
			Position scale(double s) const
			{
				return Position(m_x * s, m_y * s);
			}

			/// \brief returns this distance between this point and another point
			/// \param pos the other point
			/// \returns the distance between this point and another
			double distance(const Position &pos) const
			{
				double d = (m_x - pos.getX()) * (m_x - pos.getX()) + (m_y - pos.getY()) * (m_y - pos.getY());
				return std::sqrt(d);
			}

			/// \brief interpolate between two points
			/// The result will always between this point and the second given point
			/// \param p the second position
			/// \param x the percent distance between the point
			/// \returns the position between the current point and the given point
			Position interpolate(const Position &p, double x) const
			{
				Position ret;

				if (x <= 0.0)
				{
					ret = *this;
				}
				else if (x >= 1.0)
				{
					ret = p;
				}
				else
				{
					ret = extrapolate(p, x);
				}

				return ret;
			}

			/// \brief extrapolate between two points
			/// The result may extend on the line between the two points to any
			/// point on the traced line.
			/// \param p the second position
			/// \param x the percent distance between the point
			/// \returns the position between the current point and the given point
			Position extrapolate(const Position &p, double x) const
			{
				return Position(m_x + (p.getX() - m_x) * x, m_y + (p.getY() - m_y) * x);
			}

			/// \brief rotate the position by the rotation given
			/// \param r the rotation to apply to the position
			/// \returns a new position rotated from the origin by the rotation given
			Position rotateBy(const Rotation &r) const;

			/// \brief translate this position by the position given
			/// \param p the position to use to translate this position
			/// \returns the current position translated by the position given as an argument
			Position translateBy(const Position &p) const
			{
				return Position(m_x + p.getX(), m_y + p.getY());
			}

			/// \brief return the dot product of the two positions
			/// \param a the first object
			/// \param b the second object
			/// \returns the dot product of two 2d objects
			static double dot(const Position &a, const Position &b)
			{
				return a.getX() * b.getX() + a.getY() * b.getY();
			}

			/// \brief return the cross product of the two positions
			/// \param a the first object
			/// \param b the second object
			/// \returns the dot product of two 2d objects
			static double cross(const Position &a, const Position &b)
			{
				return a.getX() * b.getY() - a.getY() * b.getX();
			}

			/// \brief return a string representing this object
			std::string toString() const
			{
				return std::to_string(m_x) + ", " + std::to_string(m_y);
			}
		};

		inline Position operator+(const Position &a, const Position &b)
		{
			return Position(a.getX() + b.getX(), a.getY() + b.getY());
		}
	}
}
