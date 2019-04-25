#pragma once

#include "MotionProfile.h"
#include "Constraints.h"
#include "ProfileGoal.h"
#include <memory>

namespace xero
{
	namespace motion
	{
		class ProfileGenerator
		{
		private:

		public:
			ProfileGenerator() = delete;
			virtual ~ProfileGenerator() = delete;

			static std::shared_ptr<MotionProfile> generateFlippedProfile(const Constraints &con, const ProfileGoal &goal, const State &curr);
			static std::shared_ptr<MotionProfile> generateProfile(const Constraints &con, const ProfileGoal &goal, const State &curr);
		};
	}
}

