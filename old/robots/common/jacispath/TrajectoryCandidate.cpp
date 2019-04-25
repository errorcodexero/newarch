#include "TrajectoryCandidate.h"
#include <cmath>
#include <algorithm>

using namespace xero::motion;
using namespace xero::base;

namespace xero
{
	namespace jacispath
	{
		TrajectoryCandidate::TrajectoryCandidate(const std::vector<Waypoint> &waypoints, TrajectoryCandidate::FitFun fit, size_t count, double dt,
			double maxvel, double maxaccel, double maxjerk)
		{
			double totallength = 0.0;

			for (size_t i = 0; i < waypoints.size() - 1; i++)
			{
				std::shared_ptr<Spline> spline_p = std::make_shared<Spline>();
				m_splines.push_back(spline_p);

				fit(waypoints[i], waypoints[i + 1], *spline_p);

				double dist = spline_p->distance(count);
				m_endpos.push_back(dist);

				totallength += dist;
			}

			TrajectoryConfig config(dt, maxvel, maxaccel, maxjerk, 0, waypoints[0].getAngle(),
				totallength, 0, waypoints[0].getAngle(), count);
			TrajectoryInfo info(config);

			m_total_length = totallength;
			m_traj_length = info.getLength();
			m_path_length = waypoints.size();
			m_info = info;
			m_config = config;
		}

		TrajectoryCandidate::~TrajectoryCandidate()
		{
		}

		bool TrajectoryCandidate::fromSecondOrderFilter(int filter_1, int filter_2, double dt, double u, double v, 
			double impulse, int len, std::vector<Segment> &segments)
		{
			if (len < 0)
				return false;

			Segment last(dt, 0, 0, 0, u, 0, 0, 0);
			std::vector<double> f1(len);
			f1[0] = (u / v) * filter_1;
			double f2;

			for (int i = 0; i < len; i++)
			{
				double input = std::min(impulse, 1.0);

				if (input < 1)
				{
					input -= 1;
					impulse = 0.0;
				}
				else
				{
					impulse -= input;
				}

				double f1_last;

				if (i > 0)
					f1_last = f1[i - 1];
				else
					f1_last = f1[0];

				f1[i] = std::max(0.0, std::min((double)filter_1, f1_last + input));

				f2 = 0;
				for (int j = 0; j < filter_2; j++)
				{
					if (i - j < 0)
						break;

					f2 += f1[i - j];
				}
				f2 /= filter_1;

				segments[i].setVelocity(f2 / filter_2 * v);
				segments[i].setPosition((last.getVelocity() + segments[i].getVelocity()) / 2 * dt + last.getPosition());
				segments[i].setCoords(Position(segments[i].getPosition(), 0));

				segments[i].setAcceleration((segments[i].getVelocity() - last.getVelocity()) / dt);
				segments[i].setJerk((segments[i].getAcceleration() - last.getAcceleration()) / dt);
				segments[i].setDT(dt);

				last = segments[i];
			}

			return true;
		}

		bool TrajectoryCandidate::create(std::vector<Segment> &segments)
		{
			segments.resize(m_info.getLength());
			if (!fromSecondOrderFilter(m_info.GetFilter1(), m_info.GetFilter2(), m_info.getDT(), 
				m_info.getU(), m_info.getV(), m_info.getImpulse(), m_info.getLength(), segments))
				return false;

			double dtheta = m_config.getDestTheta() - m_config.getSrcTheta();
			for (int i = 0; i < m_info.getLength() ; i++)
				segments[i].setHeading(m_config.getSrcTheta() + dtheta * (segments[i].getPosition() / segments[m_info.getLength() - 1].getPosition()));

			return true;
		}

		bool TrajectoryCandidate::generate(std::vector<xero::motion::Segment> &segments)
		{
			if (!create(segments))
				return false;

			segments.resize(m_traj_length);

			double spline_pos_initial = 0;
			double spline_complete = 0;
			size_t spline_i = 0;

			for (size_t i = 0; i < m_traj_length; i++)
			{
				double pos = segments[i].getPosition();
				bool found = false;
				while (!found)
				{
					double pos_relative = pos - spline_pos_initial;
					if (pos_relative <= m_endpos[spline_i])
					{
						auto spline_p = m_splines[spline_i];
						double percentage = spline_p->progressForDistance(pos_relative, m_config.getCount());
						Position coords = spline_p->coords(percentage);
						segments[i].setHeading(spline_p->angle(percentage));
						segments[i].setCoords(coords);
						found = true;
					}
					else if (spline_i < m_path_length - 2)
					{
						spline_complete += m_endpos[spline_i];
						spline_pos_initial = spline_complete;
						spline_i++;
					}
					else
					{ 
						auto spline_p = m_splines[m_path_length - 2];
						segments[i].setHeading(spline_p->angle(1.0));
						segments[i].setCoords(spline_p->coords(1.0));
						found = true;
					}
				}
			}

			return true;
		}
	}
}
