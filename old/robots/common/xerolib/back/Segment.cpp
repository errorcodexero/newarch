#include "Segment.h"
#include <cmath>

namespace xero
{
	namespace motion
	{
		Segment::~Segment()
		{
		}

		bool Segment::isValid() const
		{
			//
			// Acceleration is required to be constant within a segment
			//
			if (std::fabs(m_start.getAccel() - m_end.getAccel()) > kDelta)
				return false;

			//
			// Velocity cannot change signs in a segment.  But if it does and one of the velocities is
			// effectively zero, that is ok
			//
			if (m_start.getVelocity() * m_end.getVelocity() < 0 && std::fabs(m_start.getVelocity()) > kDelta && std::fabs(m_end.getVelocity()) > kDelta)
					return false;

			//
			// The start position, should extrapolate to the end position
			//
			double exend = m_start.extrapolate(m_end.getTime()).getPosition();
			if (std::fabs(exend - m_end.getPosition()) > kDelta)
				return false;

			return true;
		}
	}
}
