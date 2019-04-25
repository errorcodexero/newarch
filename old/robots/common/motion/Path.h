#pragma once

#include "Segment.h"
#include <vector>
#include <string>

namespace xero
{
	namespace motion
	{
		class Path
		{
		public:
			enum PathType
			{
				Simple,
				Tank,
				Swerve,
			};

			static size_t constexpr LeftWheel = 0;
			static size_t constexpr RightWheel = 1;
			static size_t constexpr FrontLeft = 0;
			static size_t constexpr FrontRight = 0;
			static size_t constexpr BackLeft = 0;
			static size_t constexpr BackRight = 0;

		public:
			Path();
			~Path();

			bool isValid() const
			{
				bool ret = false;

				if (m_type == PathType::Simple && m_segments.size() == 1)
					ret = true;
				else if (m_type == PathType::Tank && m_segments.size() == 2)
					ret = true;
				else if (m_type == PathType::Swerve && m_segments.size() == 4)
					ret = true;

				return ret;
			}

			bool isEmpty() const
			{
				if (!isValid())
					return true;

				return m_segments.size() == 0;
			}

			PathType getType() const
			{
				return m_type;
			}

			const std::string &getErrorMessage() const
			{
				return m_error;
			}

			const std::string &getName() const
			{
				return m_name;
			}

			bool create(const std::string &name, const std::string &filename);
			bool create(const std::string &name, const std::string &filename, double width);
			bool create(const std::string &name, const std::string &filename, double width, double depth);

			size_t numberWheels() const
			{
				return m_segments.size();
			}

			size_t size() const
			{
				return m_segments[0].size();
			}

			const Segment &getSegment(size_t index) const
			{
				return m_segments[0][index];
			}

			const Segment &getSegment(size_t which, size_t index) const
			{
				return m_segments[which][index];
			}

		private:
			void modifyTank(const std::vector<Segment> &segments, double wheelbase,
				std::vector<Segment> &left, std::vector<Segment> &right);
			void modifySwerve(const std::vector<Segment> &segments, double wheelbase_width, double wheelbase_depth,
				std::vector<Segment> &frontleft, std::vector<Segment> &frontright,
				std::vector<Segment> &backleft, std::vector<Segment> &backright);

		private:
			PathType m_type;
			std::string m_name;
			std::vector<std::vector<Segment>> m_segments;
			std::string m_error;
		};
	}
}

