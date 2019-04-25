#pragma once

#include "ProfileFollower.h"

namespace xero
{
	namespace motion
	{
		class HeadingProfileFollower : public ProfileFollower
		{
		public:
			HeadingProfileFollower(double kp, double ki, double kv, double kffv, double kffa) : ProfileFollower(kp, ki, kv, kffv, kffa)
			{
			}

			virtual ~HeadingProfileFollower()
			{
			}

			virtual double update(const State &latest, double t);

			static State canonicalize(const State &st);
		};
	}
}

