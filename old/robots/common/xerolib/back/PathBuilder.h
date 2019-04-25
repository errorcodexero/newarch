#pragma once

#include "Path.h"
#include "Waypoint.h"
#include "PositionCS.h"
#include <memory>

namespace xero
{
	namespace pathfinder
	{
		class PathBuilder
		{
		private:
			static constexpr double kDelta = 1.0e-9;
			static constexpr double kReallyBig = 1e9;

		public:
			class Line
			{
			public:
				Line()
				{
					m_speed = 0.0;
				}

				Line(const Waypoint &p1, const Waypoint &p2)
				{
					m_a = p1;
					m_b = p2;

					m_slope = xero::math::Position(m_a.getPosition(), m_b.getPosition());
					m_speed = m_b.getSpeed();
					m_start = m_a.getPosition().translateBy(m_slope.scale(m_a.getRadius() / m_slope.getNorm()));
					m_end = m_b.getPosition().translateBy(m_slope.scale(-m_b.getRadius() / m_slope.getNorm()));
				}

				void addToPath(std::shared_ptr<Path> path_p, double endspeed, double maxaccel)
				{
					double len = xero::math::Position(m_end, m_start).getNorm();
					if (len > kDelta)
					{
						auto seg_p = std::make_shared<PathSegment>(m_start.getX(), m_start.getY(), m_end.getX(), m_end.getY(), 
							m_b.getSpeed(), maxaccel, path_p->getLastMotionState(), endspeed, m_b.getMarker().c_str());
						path_p->addSegment(seg_p);
					}
				}

				double getSpeed() const
				{
					return m_speed;
				}

				const xero::math::Position &getStart() const
				{
					return m_start;
				}

				const xero::math::Position &getEnd() const
				{
					return m_end;
				}

				const xero::math::Position &getSlope() const
				{
					return m_slope;
				}

			private:
				xero::math::Position m_start;
				xero::math::Position m_end;
				xero::math::Position m_slope;
				double m_speed;
				Waypoint m_a;
				Waypoint m_b;
			};
			
			class Arc
			{
			public:
				Arc(const Waypoint &a, const Waypoint &b, const Waypoint &c) : Arc(Line(a, b), Line(b, c))
				{
				}

				Arc(const Line &a, const Line &b)
				{
					m_a = a;
					m_b = b;
					m_speed = (m_a.getSpeed() + m_b.getSpeed()) / 2.0;
					m_center = intersect(a, b);
					m_radius = xero::math::Position(m_center, m_a.getEnd()).getNorm();
				}

				static xero::math::Position intersect(const Line &l1, const Line &l2)
				{
					xero::math::PositionCS linea(l1.getEnd(), xero::math::getDirection(l1.getSlope(), true).normal());
					xero::math::PositionCS lineb(l2.getStart(), xero::math::getDirection(l2.getSlope(), true).normal());
					return linea.intersection(lineb);
				}

				void addToPath(std::shared_ptr<Path> path_p, double maxaccel)
				{
					m_a.addToPath(path_p, m_speed, maxaccel);
					if (m_radius > kDelta && m_radius < kReallyBig)
					{
						auto seg_p = std::make_shared<PathSegment>(m_a.getEnd().getX(), m_a.getEnd().getY(), m_b.getStart().getX(), m_b.getStart().getY(),
							m_center.getX(), m_center.getY(), m_speed, maxaccel, path_p->getLastMotionState(), m_b.getSpeed());
						path_p->addSegment(seg_p);
					}
				}

			private:
				Line m_a;
				Line m_b;
				xero::math::Position m_center;
				double m_radius;
				double m_speed;
			} ;

		public:
			PathBuilder();
			virtual ~PathBuilder();

			static std::shared_ptr<Path> buildPath(const std::vector<Waypoint> &waypoints, double maxaccel);
		};
	}
}

