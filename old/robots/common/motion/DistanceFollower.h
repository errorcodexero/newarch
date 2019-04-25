#pragma once

#include "PathFollowerController.h"
#include "Path.h"
#include <vector>

namespace xero
{
	namespace base
	{
		class DistanceFollower : public PathFollowerController
		{
		public:
			struct FollowConfig
			{
				double p;
				double d;
				double v;
				double a;
			};

		public:
			DistanceFollower(const FollowConfig &config);
			~DistanceFollower();

			void setPath(std::shared_ptr<xero::motion::Path> path_p);

			bool isFinished() const;
			bool isValid() const;
			std::vector<double> follow(const std::vector<double> &distance);

		private:
			size_t m_segment_index;
			FollowConfig m_config;
			std::shared_ptr<xero::motion::Path> m_path_p;

			std::vector<double> m_last_error;
			std::vector<double> m_heading;
			std::vector<double> m_output;
		};

	}
}
