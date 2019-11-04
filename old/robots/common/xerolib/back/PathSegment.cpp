#include "PathSegment.h"
#include "MathUtils.h"
#include "ProfileGenerator.h"
#include "PathFinderConstants.h"

using namespace xero::math;
using namespace xero::motion;

namespace xero
{
	namespace pathfinder
	{
		PathSegment::PathSegment(double x1, double y1, double x2, double y2, double maxspeed, double maxaccel,
			const xero::motion::State &start, double endspeed)
		{
			m_start = Position(x1, y1);
			m_end = Position(x2, y2);
			m_delta_start = Position(m_start, m_end);
			m_maxspeed = maxspeed;
			m_maxaccel = maxaccel;
			m_extrapolate_lookahead = false;
			m_isline = true;
			createMotionProfiler(start, endspeed);
		}

		PathSegment::PathSegment(double x1, double y1, double x2, double y2, double maxspeed, double maxaccel,
			const xero::motion::State &start, double endspeed, const char *marker_p)
		{
			m_start = Position(x1, y1);
			m_end = Position(x2, y2);
			m_delta_start = Position(m_start, m_end);
			m_maxspeed = maxspeed;
			m_maxaccel = maxaccel;
			m_extrapolate_lookahead = false;
			m_isline = true;
			m_marker = marker_p;
			createMotionProfiler(start, endspeed);
		}

		PathSegment::PathSegment(double x1, double y1, double x2, double y2, double c1, double c2,
			double maxspeed, double maxaccel, const xero::motion::State &start, double endspeed)
		{
			m_start = Position(x1, y1);
			m_end = Position(x2, y2);
			m_center = Position(c1, c2);
			m_delta_start = Position(m_center, m_start);
			m_delta_end = Position(m_center, m_end);
			m_maxspeed = maxspeed;
			m_maxaccel = maxaccel;
			m_extrapolate_lookahead = false;
			m_isline = false;
			createMotionProfiler(start, endspeed);
		}

		PathSegment::PathSegment(double x1, double y1, double x2, double y2, double c1, double c2,
			double maxspeed, double maxaccel, const xero::motion::State &start, double endspeed, const char *marker_p)
		{
			m_start = Position(x1, y1);
			m_end = Position(x2, y2);
			m_center = Position(c1, c2);
			m_delta_start = Position(m_center, m_start);
			m_delta_end = Position(m_center, m_end);
			m_maxspeed = maxspeed;
			m_maxaccel = maxaccel;
			m_extrapolate_lookahead = false;
			m_isline = false;
			m_marker = marker_p;
			createMotionProfiler(start, endspeed);
		}

		PathSegment::~PathSegment()
		{
		}

		Position PathSegment::getClosestPoint(const xero::math::Position &pos)
		{
			Position ret;

			if (m_isline)
			{
				Position delta(m_start, m_end);
				double u = ((pos.getX() - m_start.getX()) * delta.getX() + (pos.getY() - m_start.getY()) * delta.getY()) / delta.getNormSquared();
				if (u >= 0 && u <= 1)
					ret = Position(m_start.getX() + u * delta.getX(), m_start.getY() + u * delta.getY());
				else if (u < 0)
					ret = m_start;
				else
					ret = m_end;
			}
			else
			{
				Position delta(m_center, pos);
				delta = delta.scale(m_delta_start.getNorm() / delta.getNorm());
				if (Position::cross(delta, m_delta_start) * Position::cross(delta, m_delta_end) < 0)
				{
					ret = m_center.translateBy(delta);
				}
				else
				{
					Position startdist(pos, m_start);
					Position enddist(pos, m_end);
					ret = (enddist.getNorm() < startdist.getNorm()) ? m_end : m_start;
				}
			}

			return ret;
		}

		Position PathSegment::getPointByDistance(double dist)
		{
			Position ret;
			double length = getLength();
			if (!m_extrapolate_lookahead && dist > length)
			{
				dist = length;
			}

			if (m_isline)
			{
				ret = m_start.translateBy(m_delta_start.scale(dist / length));
			}
			else
			{
				double da = getAngle(m_delta_start, m_delta_end).getRadians() * ((Position::cross(m_delta_start, m_delta_end) >= 0) ? 1 : -1);
				da *= dist / length;
				Position t = m_delta_start.rotateBy(Rotation::fromRadians(da));
				ret = m_center.translateBy(t);
			}

			return ret;
		}

		double PathSegment::getRemainingDistance(const xero::math::Position &pos)
		{
			double ret;

			if (m_isline)
			{
				ret = Position(m_end, pos).getNorm();
			}
			else
			{
				Position deltapos(m_center, pos);
				double angle = getAngle(m_delta_end, deltapos).getRadians();
				double totalangle = getAngle(m_delta_start, m_delta_end).getRadians();
				ret = angle / totalangle * getLength();
			}

			return ret;
		}

		double PathSegment::getSpeedByDistance(double dist)
		{
			double ret = 0.0;

			if (dist < m_profile->getStartPosition())
				dist = m_profile->getStartPosition();
			else if (dist > m_profile->getEndPosition())
				dist = m_profile->getEndPosition();

			auto pos_p = m_profile->firstStateByPosition(dist);
			if (pos_p != nullptr)
				ret = pos_p->getVelocity();

			return ret;
		}

		double PathSegment::getDistanceTravelled(const xero::math::Position &pos)
		{
			Position pathpos = getClosestPoint(pos);
			double remainingdist = getRemainingDistance(pathpos);
			return getLength() - remainingdist;
		}

		void PathSegment::createMotionProfiler(const xero::motion::State &start, double endspeed)
		{
			Constraints con(m_maxspeed, m_maxaccel);
			ProfileGoal goal(getLength(), endspeed);
			m_profile = ProfileGenerator::generateProfile(con, goal, start);
		}

		std::string PathSegment::toString() const
		{
			std::string ret;

			if (m_isline)
			{
				ret = "LINE";
				ret += ", start " + m_start.toString();
				ret += ", end " + m_end.toString();
				ret += ", maxspeed " + std::to_string(m_maxspeed);
			}
			else
			{
				ret = "ARC";
				ret += ", start " + m_start.toString();
				ret += ", end " + m_end.toString();
				ret += ", center " + m_center.toString();
				ret += ", maxspeed " + std::to_string(m_maxspeed);
			}

			if (m_marker.length() > 0)
				ret += ", marker '" + m_marker + "'";

			if (m_profile != nullptr)
			{
				ret += "\r\n";
				ret += m_profile->toString("        ");
			}

			return ret;
		}
	}
}
