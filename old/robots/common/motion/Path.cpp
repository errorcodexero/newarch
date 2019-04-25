#include "Path.h"
#include "SegmentIO.h"

namespace xero
{
	namespace motion
	{
		Path::Path()
		{
		}

		Path::~Path()
		{
		}

		bool Path::create(const std::string &name, const std::string &filename)
		{
			m_type = PathType::Simple;
			m_name = name;

			m_segments.resize(1);

			if (!SegmentIO::readSegments(filename, m_segments[0], m_error))
				return false;

			return true;
		}

		bool Path::create(const std::string &name, const std::string &filename, double width)
		{
			m_type = PathType::Tank;
			m_name = name;

			std::vector<Segment> segments;

			if (!SegmentIO::readSegments(filename, segments, m_error))
				return false;

			m_segments.resize(2);
			modifyTank(segments, width, m_segments[0], m_segments[1]);

			return true;
		}

		bool Path::create(const std::string &name, const std::string &filename, double width, double depth)
		{
			m_type = PathType::Swerve;
			m_name = name;

			std::vector<Segment> segments;

			if (!SegmentIO::readSegments(filename, segments, m_error))
				return false;

			m_segments.resize(4);
			modifySwerve(segments, width, depth, m_segments[0], m_segments[1], m_segments[2], m_segments[3]);

			return true;
		}

		void Path::modifyTank(const std::vector<Segment> &segments, double wheelbase,
			std::vector<Segment> &left, std::vector<Segment> &right)
		{
			double w = wheelbase / 2.0;

			for (size_t i = 0; i < segments.size(); i++)
			{
				const Segment &seg = segments[i];
				Segment leftseg = seg;
				Segment rightseg = seg;

				double cos_angle = std::cos(seg.getHeading());
				double sin_angle = std::sin(seg.getHeading());

				leftseg.setCoords(seg.getX() - (w * sin_angle), seg.getY() + (w * cos_angle));

				if (i > 0)
				{
					const Segment &last = left[i - 1];
					double distance = last.getCoords().distance(leftseg.getCoords());
					leftseg.setPosition(last.getPosition() + distance);
					leftseg.setVelocity(distance / seg.getDT());
					leftseg.setAcceleration((leftseg.getVelocity() - last.getVelocity()) / seg.getDT());
					leftseg.setJerk((leftseg.getAcceleration() - last.getAcceleration()) / seg.getDT());
				}

				rightseg.setCoords(seg.getX() + (w * sin_angle), seg.getY() - (w * cos_angle));

				if (i > 0)
				{
					const Segment &last = right[i - 1];
					double distance = last.getCoords().distance(rightseg.getCoords());
					rightseg.setPosition(last.getPosition() + distance);
					rightseg.setVelocity(distance / seg.getDT());
					rightseg.setAcceleration((rightseg.getVelocity() - last.getVelocity()) / seg.getDT());
					rightseg.setJerk((rightseg.getAcceleration() - last.getAcceleration()) / seg.getDT());
				}

				left.push_back(leftseg);
				right.push_back(rightseg);
			}
		}

		void Path::modifySwerve(const std::vector<Segment> &segments, double width, double depth,
			std::vector<Segment> &frontleft, std::vector<Segment> &frontright,
			std::vector<Segment> &backleft, std::vector<Segment> &backright)
		{
			for (size_t i = 0; i < segments.size(); i++)
			{
				const Segment &seg = segments[i];
				Segment fl = seg;
				Segment fr = seg;
				Segment bl = seg;
				Segment br = seg;

				fl.setCoords(seg.getX() - width / 2, seg.getY() + depth / 2);
				fr.setCoords(seg.getX() + width / 2, seg.getY() + depth / 2);
				bl.setCoords(seg.getX() - width / 2, seg.getY() - depth / 2);
				br.setCoords(seg.getX() + width / 2, seg.getY() - depth / 2);

				frontleft.push_back(fl);
				frontright.push_back(fr);
				backleft.push_back(bl);
				backright.push_back(br);
			}
		}
	}
}
