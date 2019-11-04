#pragma once

#include "Position.h"

namespace xero
{
	namespace math
	{
		class PositionAngle
		{
		private:
			// Position on the field
			xero::motion::Position m_position;

			// Angle in radians
			double m_angle;

		public:
			/// \brief create an object at the origin with zero angle
			PositionAngle()
			{
				m_angle = 0.0;
			}

			/// \brief create an object with the given x and y position and an angle of zero
			/// \param the x coordinate of the object
			/// \param the y coordinate of the object
			PositionAngle(double x, double y)
			{
				m_position.setX(x);
				m_position.setY(y);
				m_angle = 0;
			}

			/// \brief create an object with the given position and an angle of zero
			/// \param pos the position of the object
			PositionAngle(const xero::motion::Position &pos)
			{
				m_position = pos;
				m_angle = 0.0;
			}

			/// \brief create an object with the given position and angle
			/// \param pos the position of the object
			/// \param angle the angle of the object
			PositionAngle(const xero::motion::Position &pos, double angle)
			{
				m_position = pos;
				m_angle = angle;
			}

			/// \brief create an object with the given position and angle
			/// \param x the x position of the object
			/// \param y the y position of the object
			/// \param angle the rotation angle of the object
			PositionAngle(double x, double y, double angle)
			{
				m_position.setX(x);
				m_position.setY(y);
				m_angle = angle;
			}

			/// \brief destroy object
			virtual ~PositionAngle()
			{
			}

			/// \brief get the position portion of the object
			/// \returns the position object associated with this object
			const xero::motion::Position &getPosition() const
			{
				return m_position;
			}

			/// \brief get the angle portion of the object
			/// \returns the angle portion of the object
			double getAngle() const
			{
				return m_angle;
			}

			/// \brief return the x component of the position
			/// \returns the x component of the position
			double getX() const
			{
				return m_position.getX();
			}

			/// \brief return the y component of the position
			/// \returns the y component of the position
			double getY() const
			{
				return m_position.getY();
			}

			/// \brief return a scaled version of this object
			/// \param scale the scale factor to apply
			/// \returns a scaled version of this object
			PositionAngle scaled(double scale) const
			{
				return PositionAngle(getX() * scale, getY() * scale, getAngle() * scale);
			}

			std::string toString() const
			{
				return m_position.toString() + ", angle " + std::to_string(m_angle);
			}
		};
	}
}
