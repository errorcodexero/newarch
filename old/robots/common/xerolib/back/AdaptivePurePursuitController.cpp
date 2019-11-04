#include "AdaptivePurePursuitController.h"
#include <algorithm>
#include <iostream>

using namespace xero::math;

namespace xero
{
	namespace pathfinder
	{
		AdaptivePurePursuitController::Command AdaptivePurePursuitController::update(const PositionCS &p, xero::pathfinder::PathDebugData &debug)
		{
			PositionCS pose(p);
			if (m_reversed)
				pose = PositionCS(p.getPos(), p.getRotation().rotateBy(Rotation::fromDegrees(180.0)));

			debug.m_segment_index = m_path_p->getSegmentIndex();
			auto seg_p = m_path_p->getCurrentSegment();
			Path::TargetPointReport report = m_path_p->getTargetPoint(pose.getPos(), m_lookahead);
			if (isFinished())
			{
				PositionAngle pa;
				return Command(pa, report.ClosestPointDistance, report.MaxSpeed, 0.0, report.LookAheadPoint, report.RemainingPathDistance);
			}

			debug.m_segment_remaining = report.RemainingSegmentDistance;
			debug.m_closest_x = report.ClosestPoint.getX();
			debug.m_closest_y = report.ClosestPoint.getY();
			debug.m_seg_start_x = seg_p->getStart().getX();
			debug.m_seg_start_y = seg_p->getStart().getY();
			debug.m_seg_end_x = seg_p->getEnd().getX();
			debug.m_seg_end_y = seg_p->getEnd().getY();

			Arc arc(pose, report.LookAheadPoint);
			debug.arc_center_x = arc.Center.getX();
			debug.arc_center_y = arc.Center.getY();
			debug.arc_radius = arc.Radius;
			debug.arc_length = arc.Length;
			double scale_factor = 1.0;

			if (report.LookAheadPointSpeed < kDelta && report.RemainingPathDistance < arc.Length)
			{
				//
				// The look ahead speed is zero and we are on the last segment of the
				// path.  In other words, we are winding down to a stop.
				//
				scale_factor = std::max(0.0, report.RemainingPathDistance / arc.Length);
				m_at_end = true;
			}
			else
			{
				m_at_end = false;
			}

			if (m_reversed)
				scale_factor *= -1.0;

			double angle = arc.Length * getDirection(pose, report.LookAheadPoint) * std::fabs(scale_factor) / arc.Radius;
			PositionAngle pa(scale_factor * arc.Length, 0.0, angle);
			return Command(pa, report.ClosestPointDistance, report.MaxSpeed, std::copysign(report.LookAheadPointSpeed, scale_factor), 
				report.LookAheadPoint, report.RemainingPathDistance);
		}
	}
}
