#pragma once

#include "Position.h"
#include "PositionAngle.h"
#include "PositionCS.h"
#include "Path.h"
#include "MathUtils.h"
#include "PathFinderConstants.h"
#include "PathDebugData.h"
#include <memory>

namespace xero
{
	namespace pathfinder
	{
		class AdaptivePurePursuitController
		{
		private:
			static constexpr double kPI = 3.14159265359;
			static constexpr double kDelta = 1e-6;

		public:
			class Command
			{
			public:
				xero::math::PositionAngle m_delta;
				double m_cross_track_error;
				double m_max_velocity;
				double m_end_velocity;
				xero::math::Position m_lookahead_point;
				double m_remaining_path_length;

			public:
				Command()
				{
				}

				Command(const xero::math::PositionAngle &pa, double cte, double maxvel, double endvel, 
					const xero::math::Position &lap, double remain)
				{
					m_delta = pa;
					m_cross_track_error = cte;
					m_max_velocity = maxvel;
					m_end_velocity = endvel;
					m_lookahead_point = lap;
					m_remaining_path_length = remain;
				}
			};

			class Arc
			{
			public:
				xero::math::Position Center;
				double Radius;
				double Length;

				Arc(const xero::math::PositionCS &pose, const xero::math::Position &pt)
				{
					Center = getCenter(pose, pt);
					Radius = xero::math::Position(Center, pt).getNorm();
					Length = getLength(pose, pt, Center, Radius);
				}
			};

		public:
			AdaptivePurePursuitController(std::shared_ptr<Path> path_p, bool reversed, const Lookahead &la)
			{
				m_path_p = path_p;
				m_reversed = reversed;
				m_lookahead = la;
				m_at_end = false;
			}

			virtual ~AdaptivePurePursuitController()
			{
			}

			Command update(const xero::math::PositionCS &pos, xero::pathfinder::PathDebugData &debug);

			bool hasPassedMarker(const char *marker_p) const
			{
				return m_path_p->hasPassedMarker(marker_p);
			}

			static xero::math::Position getCenter(const xero::math::PositionCS &pose, const xero::math::Position &pos)
			{
				xero::math::Position ret;

				xero::math::Position poseToHalfway = pose.getPos().interpolate(pos, 0.5);
				xero::math::Rotation normal = xero::math::getDirection(pose.getPos().inverse().translateBy(poseToHalfway)).normal();
				xero::math::PositionCS perpBisector(poseToHalfway, normal);
				xero::math::PositionCS normalFromPose(pose.getPos(), pose.getRotation().normal());

				if (normalFromPose.isColinear(perpBisector.normal()))
				{
					ret = poseToHalfway;
				}
				else
				{
					ret = normalFromPose.intersection(perpBisector);
				}

				return ret;
			}

			static double getRadius(const xero::math::PositionCS &pose, const xero::math::Position &pos)
			{
				xero::math::Position center = getCenter(pose, pos);
				return xero::math::Position(center, pos).getNorm();
			}

			static double getLength(const xero::math::PositionCS &pose, const xero::math::Position &pos)
			{
				double radius = getRadius(pose, pos);
				xero::math::Position center = getCenter(pose, pos);
				return getLength(pose, pos, center, radius);
			}

			static double getLength(const xero::math::PositionCS &pose, const xero::math::Position &pos,
				const xero::math::Position &center, double radius)
			{
				double ret = 0;

				if (radius < xero::pathfinder::Constants::kMaxAdaptivePursuitArcRadius)
				{
					xero::math::Position centerToPoint(center, pos);
					xero::math::Position centerToPose(center, pose.getPos());
					xero::math::Rotation r = pose.getRotation().normal();
					double v = xero::math::Position::cross(xero::math::Position(r.getCos(), r.getSin()), xero::math::Position(pose.getPos(), pos));
					bool behind = xero::math::getSign(v) > 0.0;
					xero::math::Rotation angle = xero::math::getAngle(centerToPose, centerToPoint);
					if (behind)
						ret = 2.0 * kPI - std::fabs(angle.getRadians());
					else
						ret = std::fabs(angle.getRadians());
				}
				else
				{
					ret = xero::math::Position(pose.getPos(), pos).getNorm();
				}

				return ret;
			}

			static int getDirection(const xero::math::PositionCS &pose, const xero::math::Position &pos)
			{
				xero::math::Position poseToPoint(pose.getPos(), pos);
				xero::math::Position robot(pose.getRotation().getCos(), pose.getRotation().getSin());
				double cross = robot.getX() * poseToPoint.getY() - robot.getY() * poseToPoint.getX();
				return (cross < 0) ? -1 : 1;
			}

			bool isFinished() const
			{
				return m_at_end;
			}

		private:
			//
			// The path we are following
			//
			std::shared_ptr<Path> m_path_p;

			//
			// If true, we are at the end of the path
			//
			bool m_at_end;

			//
			// If true, we are following the path in reverse
			//
			bool m_reversed;

			//
			// The lookahead
			//
			Lookahead m_lookahead;
		};
	}
}

