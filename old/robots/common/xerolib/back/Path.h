#pragma once

#include "PathSegment.h"
#include "Lookahead.h"
#include "PathFinderConstants.h"
#include <vector>
#include <list>
#include <set>
#include <memory>
#include <iostream>

namespace xero
{
	namespace pathfinder
	{
		class Path
		{
		private:
			static constexpr double kSegmentCompletionTolerance = 1.0e-9;

		public:
			class TargetPointReport
			{
			public:
				double MaxSpeed;
				xero::math::Position ClosestPoint;
				double ClosestPointDistance;
				double ClosestPointSpeed;
				xero::math::Position LookAheadPoint;
				double LookAheadPointSpeed;
				double RemainingSegmentDistance;
				double RemainingPathDistance;
			};
			
		private:
			std::vector<std::shared_ptr<PathSegment>> m_segments;
			std::shared_ptr<PathSegment> m_prev_segment;
			std::list<std::string> m_markers_crossed;
			double m_maxaccel;

		public:
			Path(double maxaccel);
			virtual ~Path();

			size_t getSegmentIndex() const
			{
				if (m_segments.size() == 0)
					return std::numeric_limits<size_t>::max();

				return m_segments[0]->getIndex();
			}

			std::shared_ptr<PathSegment> getCurrentSegment()
			{
				if (m_segments.size() == 0)
					return nullptr;

				return m_segments[0];
			}

			void extrapolateLast()
			{
				std::shared_ptr<PathSegment> last_p = m_segments[m_segments.size() - 1];
				last_p->extrapolateLookahead(true);
			}

			xero::math::Position getEndPosition() const
			{
				return m_segments[m_segments.size() - 1]->getEnd();
			}

			void addSegment(std::shared_ptr<PathSegment> seg_p)
			{
				seg_p->setIndex(m_segments.size());
				m_segments.push_back(seg_p);
			}

			xero::motion::State getLastMotionState() const
			{
				xero::motion::State state;

				if (m_segments.size() > 0)
				{
					state = m_segments[m_segments.size() - 1]->getEndState();
					state = xero::motion::State(0.0, 0.0, state.getVelocity(), state.getAccel());
				}

				return state;
			}

			double getSegmentRemainingDist(const xero::math::Position &pos)
			{
				auto seg_p = m_segments[0];
				return seg_p->getRemainingDistance(seg_p->getClosestPoint(pos));
			}

			double getSegmentLength()
			{
				auto seg_p = m_segments[0];
				return seg_p->getLength();
			}

			TargetPointReport getTargetPoint(const xero::math::Position &robot, const Lookahead &ahead);

			double getSpeed(const xero::math::Position &pos) const
			{
				return m_segments[0]->getSpeedByClosestPoint(pos);
			}

			void checkSegmentDone(const xero::math::Position &pos)
			{
				auto seg_p = m_segments[0];
				auto closepos = seg_p->getClosestPoint(pos);
				double remaining = seg_p->getRemainingDistance(closepos);
				if (remaining < Constants::kSegmentCompletionTolerance)
					removeCurrentSegment();
			}

			void removeCurrentSegment()
			{
				auto seg_p = m_segments[0];
				if (seg_p->getMarker().length() > 0)
					m_markers_crossed.push_back(seg_p->getMarker());
				m_segments.erase(m_segments.begin());
			}

			bool hasPassedMarker(const char *marker_p) const
			{
				auto it = std::find(m_markers_crossed.begin(), m_markers_crossed.end(), marker_p);
				return it != m_markers_crossed.end();
			}

			void verifySpeeds();

			std::string toString() const;
		};
	}
}

