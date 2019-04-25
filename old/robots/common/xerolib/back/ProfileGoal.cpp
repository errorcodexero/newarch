#include "ProfileGoal.h"
#include "State.h"
#include <limits>

namespace xero
{
	namespace motion
	{
		ProfileGoal::ProfileGoal()
		{
			m_position = 0.0;
			m_max_abs_velocity = std::numeric_limits<double>::infinity();
			m_pos_tolerance = 1e-3;
			m_vel_tolerance = 1e-2;
			m_type = CompletionType::Overshoot;
			sanity();
		}

		ProfileGoal::ProfileGoal(double pos)
		{
			m_position = pos;
			m_max_abs_velocity = 0.0;
			m_pos_tolerance = 1e-3;
			m_vel_tolerance = 1e-2;
			m_type = CompletionType::Overshoot;
			sanity();
		}

		ProfileGoal::ProfileGoal(double pos, double maxvel)
		{
			m_position = pos;
			m_max_abs_velocity = maxvel;
			m_pos_tolerance = 1e-3;
			m_vel_tolerance = 1e-2;
			m_type = CompletionType::Overshoot;
			sanity();
		}

		ProfileGoal::ProfileGoal(double pos, double maxvel, CompletionType type)
		{
			m_position = pos;
			m_max_abs_velocity = maxvel;
			m_pos_tolerance = 1e-3;
			m_vel_tolerance = 1e-2;
			m_type = type;
			sanity();
		}

		ProfileGoal::ProfileGoal(double pos, double maxvel, CompletionType type, double postol, double veltol)
		{
			m_position = pos;
			m_max_abs_velocity = maxvel;
			m_pos_tolerance = postol;
			m_vel_tolerance = veltol;
			m_type = type;
			sanity();
		}

		ProfileGoal::~ProfileGoal()
		{
		}

		bool ProfileGoal::atGoalState(const State &st) const
		{
			//
			// If the position is not right, we are not at the given state
			//
			if (!atGoalPosition(st.getPosition()))
				return false;

			//
			// If the position is right and we are allowed to exceed the desired velocity, we 
			// are there
			//
			if (m_type == CompletionType::ExceedMaxVelocity)
				return true;

			//
			// If the velocity is in the desired range, we are there
			//
			if (std::fabs(st.getVelocity()) < m_max_abs_velocity + m_vel_tolerance)
				return true;

			//
			// Nope, position is right but the velocity is not right
			//
			return false;
		}
	}
}
