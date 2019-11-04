#pragma once

#include "Position.h"

namespace xero
{
	namespace pathfinder
	{
		class Waypoint
		{
		public:
			Waypoint()
			{
				m_radius = 0.0;
				m_speed = 0.0;
			}

			Waypoint(double x, double y, double r, double s)
			{
				m_position = xero::math::Position(x, y);
				m_radius = r;
				m_speed = s;
			}

			Waypoint(double x, double y, double r, double s, const char *marker_p)
			{
				m_position = xero::math::Position(x, y);
				m_radius = r;
				m_speed = s;
				m_marker = marker_p;
			}

			Waypoint(const xero::math::Position &pos, double r, double s)
			{
				m_position = pos;
				m_radius = r;
				m_speed = s;
			}

			Waypoint(const xero::math::Position &pos, double r, double s, const char *marker_p)
			{
				m_position = pos;
				m_radius = r;
				m_speed = s;
				m_marker = marker_p;
			}

			virtual ~Waypoint()
			{
			}

			const xero::math::Position &getPosition() const
			{
				return m_position;
			}

			double getSpeed() const
			{
				return m_speed;
			}

			double getRadius() const
			{
				return m_radius;
			}

			bool hasMarker() const
			{
				return m_marker.length() > 0;
			}

			const std::string &getMarker() const
			{
				return m_marker;
			}

		private:
			xero::math::Position m_position;
			double m_radius;
			double m_speed;
			std::string m_marker;
		};
	}
}
