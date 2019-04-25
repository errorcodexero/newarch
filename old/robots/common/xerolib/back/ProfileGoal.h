#pragma once

#include <string>
#include <cmath>

namespace xero
{
	namespace motion
	{
		class State;

		class ProfileGoal
		{
		private:
			static constexpr double kDelta = 1.0e-9;

		public:
			enum CompletionType
			{
				Overshoot,
				ExceedMaxAccel,
				ExceedMaxVelocity
			};

		private:
			double m_position;
			double m_max_abs_velocity;
			CompletionType m_type;
			double m_pos_tolerance;
			double m_vel_tolerance;

		public:
			ProfileGoal();
			ProfileGoal(double pos);
			ProfileGoal(double pos, double maxvel);
			ProfileGoal(double pos, double maxvel, CompletionType type);
			ProfileGoal(double pos, double maxvel, CompletionType type, double postol, double veltol);
			~ProfileGoal();

			double getPosition() const
			{
				return m_position;
			}

			double getMaxVelocity() const
			{
				return m_max_abs_velocity;
			}

			CompletionType getType() const
			{
				return m_type;
			}

			double getPositionTolerance() const
			{
				return m_pos_tolerance;
			}

			double getVelocityTolerance() const
			{
				return m_vel_tolerance;
			}

			ProfileGoal flipped() const
			{
				return ProfileGoal(-m_position, m_max_abs_velocity, m_type, m_pos_tolerance, m_vel_tolerance);
			}

			bool atGoalState(const State &st) const;
			bool atGoalPosition(double pos) const
			{
				return std::fabs(pos - m_position) < m_pos_tolerance;
			}

			std::string toString() const
			{
				std::string ret("ProfileGoal: ");
				ret += "pos " + std::to_string(m_position);
				ret += " +/- " + std::to_string(m_pos_tolerance);
				ret += ", maxvel " + std::to_string(m_max_abs_velocity);
				ret += " +/- " + std::to_string(m_vel_tolerance);

				switch (m_type)
				{
				case CompletionType::Overshoot:
					ret += ", type Overshoot";
					break;
				case CompletionType::ExceedMaxAccel:
					ret += ", type ExceedMaxAccel";
					break;
				case CompletionType::ExceedMaxVelocity:
					ret += ", type ExceedMaxVelocity";
					break;
				default:
					ret += ", type UNKNOWN";
					break;
				}

				return ret;
			}

			bool operator==(const ProfileGoal &g) const
			{
				if (std::fabs(m_position - g.getPosition()) > kDelta)
					return false;

				if (std::fabs(m_max_abs_velocity - g.getMaxVelocity()) > kDelta)
					return false;

				if (m_type != g.getType())
					return false;

				if (std::fabs(m_pos_tolerance - g.getPositionTolerance()) > kDelta)
					return false;

				if (std::fabs(m_vel_tolerance - g.getVelocityTolerance()) > kDelta)
					return false;

				return true;
			}

			bool operator!=(const ProfileGoal &g) const
			{
				return !(*this == g);
			}

		private:
			void sanity()
			{
				if (m_max_abs_velocity > m_vel_tolerance && m_type == CompletionType::Overshoot)
					m_type = CompletionType::ExceedMaxVelocity;
			}
		};
	}
}