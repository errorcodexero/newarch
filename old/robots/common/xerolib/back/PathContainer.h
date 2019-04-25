#pragma once

#include <string>
#include <memory>
#include "PositionCS.h"
#include "Path.h"

namespace xero
{
	namespace pathfinder
	{
		class PathContainer
		{
		public:
			PathContainer(const char *name_p = nullptr)
			{
				if (name_p != nullptr)
					m_name = name_p;
			}

			virtual ~PathContainer()
			{
			}

			virtual std::string getName() const
			{
				return m_name;
			}

			virtual std::shared_ptr<Path> buildPath(double maxaccel) = 0;
			virtual xero::math::PositionCS getStartPose() = 0;
			virtual bool isReversed() = 0;

		protected:
			void setName(const char *name_p)
			{
				m_name = name_p;
			}

			void setName(const std::string &name)
			{
				m_name = name;
			}

			double deg2rad(double deg)
			{
				return deg / 180.0 * PI;
			}
			static constexpr double PI = 3.1415926;

		private:
			std::string m_name;
		};
	}
}

