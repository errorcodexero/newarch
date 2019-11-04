#pragma once

#include "Segment.h"
#include <vector>
#include <list>
#include <stdexcept>
#include <string>
#include <cmath>
#include <memory>
#include <iterator>

namespace xero
{
	namespace motion
	{
		class MotionProfile
		{
		private:
			static constexpr double kDelta = 1.0e-6;

		private:
			std::vector<Segment> m_segments;

		public:
			MotionProfile();
			MotionProfile(const std::list<Segment> &segments);
			MotionProfile(const std::vector<Segment> &segments);
			virtual ~MotionProfile();

			std::vector<Segment> &getSegments()
			{
				return m_segments;
			}

			const std::vector<Segment> &getSegments() const
			{
				return m_segments;
			}

			bool isEmpty() const
			{
				return m_segments.size() == 0;
			}

			void clear()
			{
				m_segments.clear();
			}

			void reset(State st)
			{
				clear();
				m_segments.push_back(Segment(st, st));
			}

			const State &getStartState() const
			{
				if (isEmpty())
				{
					std::string msg("cannot getStartState() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				return m_segments[0].getStart();
			}

			double getStartTime() const
			{
				if (isEmpty())
				{
					std::string msg("cannot getStartTime() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				return m_segments[0].getStart().getTime();
			}

			double getStartPosition() const
			{
				if (isEmpty())
				{
					std::string msg("cannot getStartPosition() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				return m_segments[0].getStart().getPosition();
			}

			const State &getEndState() const
			{
				if (isEmpty())
				{
					std::string msg("cannot getEndState() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				return m_segments[m_segments.size() - 1].getEnd();
			}

			double getEndTime() const
			{
				if (isEmpty())
				{
					std::string msg("cannot getEndTime() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				return m_segments[m_segments.size() - 1].getEnd().getTime();
			}

			double getEndPosition() const
			{
				if (isEmpty())
				{
					std::string msg("cannot getEndPosition() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				return m_segments[m_segments.size() - 1].getEnd().getPosition();
			}

			double getDuration() const
			{
				if (isEmpty())
					return std::nan("");

				return getEndTime() - getStartTime();
			}

			double getLength() const
			{
				double length = 0;
				for (const Segment &seg : m_segments)
					length += seg.length();

				return length;
			}

			bool isValid() const;

			std::shared_ptr<State> stateByTime(double t);

			State stateByTimeClamped(double t);

			std::shared_ptr<State> firstStateByPosition(double pos);

			void trimBeforeTime(double t);

			void consolidate();

			void append(double acc, double dt)
			{
				if (isEmpty())
				{
					std::string msg("cannot append() on an empty profile");
					std::runtime_error err(msg);
					throw err;
				}

				const State &last_end = m_segments[m_segments.size() - 1].getEnd();
				State newst(last_end.getTime(), last_end.getPosition(), last_end.getVelocity(), acc);
				Segment seg(newst, newst.extrapolate(newst.getTime() + dt));
				m_segments.push_back(seg);
			}

			void append(const Segment &seg)
			{
				m_segments.push_back(seg);
			}

			void append(const MotionProfile &p)
			{
				for (const Segment &seg : p.m_segments)
					m_segments.push_back(seg);
			}

			std::string toString() const
			{
				std::string str;
				return toString(str);
			}

			std::string toString(const std::string &prefix) const
			{
				std::string ret;

				for (const Segment &seg : m_segments)
				{
					ret += prefix ;
					ret += "Start: " + seg.getStart().toString();
					ret += "\r\n";
					ret += prefix ;
					ret += "End: " + seg.getEnd().toString();
					ret += "\r\n";
				}

				return ret;
			}
		};
	}
}

