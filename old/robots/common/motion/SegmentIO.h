#pragma once

#include "Segment.h"
#include <vector>
#include <string>

namespace xero
{
	namespace motion
	{
		class SegmentIO
		{
		public:
			SegmentIO() = delete;
			~SegmentIO() = delete;

			static bool writeSegments(const std::string &filename, const std::vector<xero::motion::Segment> &segments);
			static bool readSegments(const std::string &filename, std::vector<xero::motion::Segment> &segments, std::string &error);

		private:
		};
	}
}
