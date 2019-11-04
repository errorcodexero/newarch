#include "DistanceFollower.h"
#include "Path.h"
#include "Segment.h"
#include <iostream>

using namespace xero::motion;

namespace xero
{
	namespace base
	{
		DistanceFollower::DistanceFollower(const FollowConfig &config)
		{
			m_segment_index = 0;
			m_config = config;

		}

		DistanceFollower::~DistanceFollower()
		{
		}

		void DistanceFollower::setPath(std::shared_ptr<Path> path_p)
		{
			m_path_p = path_p;

			for (size_t i = 0; i < m_path_p->numberWheels(); i++)
			{
				m_last_error.push_back(0.0);
				m_heading.push_back(0.0);
				m_output.push_back(0.0);
			}
		}

		bool DistanceFollower::isFinished() const
		{
			if (m_path_p == nullptr || !m_path_p->isValid())
				return true;

			return m_segment_index >= m_path_p->size();
		}

		bool DistanceFollower::isValid() const
		{
			return m_path_p->isValid();
		}

		std::vector<double> DistanceFollower::follow(const std::vector<double> &distance)
		{
			std::vector<double> ret;

			for (size_t i = 0; i < m_path_p->numberWheels() ; i++)
			{
				std::cout << "Segment " << m_segment_index << std::endl;
				if (isFinished())
				{
					ret.push_back(0.0);

				}
				else
				{
					const Segment &seg = m_path_p->getSegment(i, m_segment_index);
					double error = seg.getPosition() - distance[i];
					double calc = m_config.p * error +
						m_config.d * (error - m_last_error[i]) / seg.getDT() +
						m_config.v * seg.getVelocity() + m_config.a * seg.getAcceleration();

					m_last_error[i] = error;
					m_heading[i] = seg.getHeading();
					m_output[i] = calc;
					m_segment_index++;

					ret.push_back(calc);
				}
			}
			return ret;
		}
	}
}
