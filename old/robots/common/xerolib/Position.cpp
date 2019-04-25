#include "Position.h"
#include "Rotation.h"

namespace xero
{
	namespace base
	{
		Position Position::rotateBy(const Rotation &r) const
		{
			return Position(m_x * r.getCos() - m_y * r.getSin(), m_x * r.getSin() + m_y * r.getCos());
		}
	}
}
