#pragma once

#include "Position.h"
#include "MotionProfile.h"
#include "MathUtils.h"
#include <memory>
#include <string>

namespace xero
{
	namespace pathfinder
	{
		class PathSegment
		{
		private:
			xero::math::Position m_start;
			xero::math::Position m_end;
			xero::math::Position m_center;
			xero::math::Position m_delta_start;
			xero::math::Position m_delta_end;
			size_t m_index;
			double m_maxspeed;
			double m_maxaccel;
			bool m_isline;
			std::shared_ptr<xero::motion::MotionProfile> m_profile;
			bool m_extrapolate_lookahead;
			std::string m_marker;

		public:
			PathSegment(double x1, double y1, double x2, double y2, double maxspeed, double maxaccel,
				const xero::motion::State &start, double endspeed);
			PathSegment(double x1, double y1, double x2, double y2, double maxspeed, double maxaccel,
				const xero::motion::State &start, double endspeed, const char *marker_p);
			PathSegment(double x1, double y1, double x2, double y2, double c1, double c2,
				double maxspeed, double maxaccel, const xero::motion::State &start, double endspeed);
			PathSegment(double x1, double y1, double x2, double y2, double c1, double c2, 
				double maxspeed, double maxaccel, const xero::motion::State &start, double endspeed, const char *marker_p);
			virtual ~PathSegment();

			size_t getIndex() const
			{
				return m_index;
			}

			void setIndex(size_t i)
			{
				m_index = i;
			}

			double getMaxSpeed() const
			{
				return m_maxspeed;
			}

			const xero::math::Position &getStart() const
			{
				return m_start;
			}

			const xero::math::Position &getEnd() const
			{
				return m_end;
			}

			double getLength() const
			{
				double ret = 0.0;

				if (m_isline)
					ret = m_delta_start.getNorm();
				else
					ret = m_delta_start.getNorm() * xero::math::getAngle(m_delta_start, m_delta_end).getRadians();

				return ret;
			}

			void extrapolateLookahead(bool v)
			{
				m_extrapolate_lookahead = v;
			}

			xero::math::Position getClosestPoint(const xero::math::Position &pos);
			xero::math::Position getPointByDistance(double dist);
			double getRemainingDistance(const xero::math::Position &pos);
			double getSpeedByDistance(double dist);
			double getSpeedByClosestPoint(const xero::math::Position &pos)
			{
				return getSpeedByDistance(getDistanceTravelled(pos));
			}

			const xero::motion::State &getEndState() const
			{
				return m_profile->getEndState();
			}

			const xero::motion::State &getStartState() const
			{
				return m_profile->getStartState();
			}

			const std::string &getMarker() const
			{
				return m_marker;
			}

			std::string toString() const;

			void createMotionProfiler(const xero::motion::State &start, double endspeed);

		private:
			double getDistanceTravelled(const xero::math::Position &pos);

		};
	}
}

