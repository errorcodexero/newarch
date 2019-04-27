#include "TimingUtil.h"
#include "Trajectory.h"
#include "ConstrainedPoint.h"
#include <cmath>
#include <algorithm>
#include <cassert>

namespace xero {
	namespace paths {

		TimedTrajectory TimingUtil::timeParameterizeTrajectory(bool reverse, DistanceView& distview, const ConstraintList& constraints, double step_size,
			double startvel, double endvel, double maxvel, double maxaccel)
		{
			size_t num_states = static_cast<size_t>(std::ceil(distview.length() / step_size)) + 1 ;
			std::vector<TrajectorySamplePoint> points;

			for (size_t i = 0; i < num_states; i++)
			{
				double t = std::min(i * step_size, distview.length());
				TrajectorySamplePoint pt = distview.sample(t);
				points.push_back(pt);
			}

			return timeParameterizeTrajectory(reverse, points, constraints, startvel, endvel, maxvel, maxaccel);
		}

		TimedTrajectory TimingUtil::timeParameterizeTrajectory(bool reverse, std::vector<TrajectorySamplePoint> &states, const ConstraintList& constraints,
			double startvel, double endvel, double maxvel, double maxaccel)
		{
			std::vector<ConstrainedPoint> constrained_states;
			ConstrainedPoint pred(&states[0], 0.0, startvel, -maxaccel, maxaccel);

			//
			// Forward pass
			//
			for (size_t i = 0; i < states.size(); i++)
			{
				ConstrainedPoint cs(&states[i]);
				double ds = cs.getState()->distance(*pred.getState());
				cs.setDistance(pred.getDistance() + ds);

				while (true)
				{
					double calcvel = std::sqrt(pred.getMaxVelocity() * pred.getMaxVelocity() + 2.0 * pred.getMaxAccel() * ds);
					double mxv = std::min(maxvel, calcvel);
					assert(!std::isnan(mxv));

					cs.setMaxVelocity(mxv);
					cs.setMaxAccel(maxaccel);
					cs.setMinAccel(-maxaccel);

					//
					// See if any of the constraints cause us to reduce the velocity
					//
					for (auto constraint : constraints)
					{
						double vel = std::min(constraint->getMaxVelocity(cs), cs.getMaxVelocity());
						cs.setMaxVelocity(vel);
					}
					assert(cs.getMaxVelocity() >= 0.0);

					//
					// See if there are any acceleration based constraints
					//
					for (auto constraint : constraints)
					{
						double vel = reverse ? -cs.getMaxVelocity() : cs.getMaxVelocity();
						MinMaxAcceleration acc = constraint->getMinMacAccel(cs, vel);
						assert(acc.isValid());

						double tmp = std::max(cs.getMinAccel(), reverse ? -acc.getMaxAccel() : acc.getMinAccel());
						cs.setMinAccel(tmp);

						tmp = std::min(cs.getMaxAccel(), reverse ? -acc.getMinAccel() : acc.getMaxAccel());
						cs.setMinAccel(tmp);
					}
					assert(cs.getMaxAccel() >= cs.getMinAccel());

					if (ds < kEpsilon)
						break;

					double actual_acceleration = (cs.getMaxVelocity() * cs.getMaxVelocity() - pred.getMaxVelocity() * pred.getMaxVelocity()) / (2.0 * ds);
					if (cs.getMaxAccel() < actual_acceleration - kEpsilon) {
						pred.setMaxAccel(cs.getMaxAccel());
						if (i != 0)
							constrained_states[i - 1] = pred;
					}
					else {
						if (actual_acceleration > pred.getMinAccel() + kEpsilon)
						{
							pred.setMaxAccel(actual_acceleration);
							if (i != 0)
								constrained_states[i - 1] = pred;
						}
						break;
					}
				}
				constrained_states.push_back(cs);
				pred = cs;
			}

			// Backward pass
			ConstrainedPoint succ(&states[states.size() - 1], constrained_states[states.size() - 1].getDistance(), startvel, -maxaccel, maxaccel);
			for (size_t i = states.size() - 1; i < states.size(); --i)		// Counting down, when unsigned int wraps, we are done
			{
				ConstrainedPoint& cs = constrained_states[i];
				double ds = cs.getDistance() - succ.getDistance();

				while (true)
				{
					// Enforce reverse max reachable velocity limit.
					// vf = sqrt(vi^2 + 2*a*d), where vi = successor.
					double new_max_velocity = std::sqrt(succ.getMaxVelocity() * succ.getMaxVelocity() + 2.0 * succ.getMinAccel() * ds);
					if (new_max_velocity >= cs.getMaxVelocity())
						break;

					assert(!std::isnan(new_max_velocity));
					cs.setMaxVelocity(new_max_velocity);

					//
					// See if there are any acceleration based constraints
					//
					for (auto constraint : constraints)
					{
						double vel = reverse ? -cs.getMaxVelocity() : cs.getMaxVelocity();
						MinMaxAcceleration acc = constraint->getMinMacAccel(cs, vel);
						assert(acc.isValid());

						double tmp = std::max(cs.getMinAccel(), reverse ? -acc.getMaxAccel() : acc.getMinAccel());
						cs.setMinAccel(tmp);

						tmp = std::min(cs.getMaxAccel(), reverse ? -acc.getMinAccel() : acc.getMaxAccel());
						cs.setMinAccel(tmp);
					}
					assert(cs.getMaxAccel() >= cs.getMinAccel());

					if (ds > kEpsilon) {
						break;
					}

					// If the min acceleration for this constraint state is more conservative than what we have applied, we
					// need to reduce the min accel and try again.
					// TODO: Simply using the new min acceleration is guaranteed to be valid, but may be too conservative.
					// Doing a search would be better.
					double actual_acceleration = (cs.getMaxVelocity() * cs.getMaxVelocity() - succ.getMaxVelocity() * succ.getMaxVelocity()) / (2.0 * ds);
					if (cs.getMinAccel() > actual_acceleration + kEpsilon) {
						succ.setMinAccel(cs.getMinAccel());
						if (i != constrained_states.size() - 1)
							constrained_states[i + 1] = succ;
					}
					else {
						succ.setMinAccel(actual_acceleration);
						if (i != constrained_states.size() - 1)
							constrained_states[i + 1] = succ;
						break;
					}
				}
				succ = cs;
			}

			// Integrate the constrained states forward in time to obtain the TimedStates
			std::vector<TimedTrajectoryPoint> timed;
			double t = 0.0;
			double s = 0.0;
			double v = 0.0;
			for (size_t i = 0; i < states.size(); i++)
			{
				const ConstrainedPoint& cs = constrained_states[i];
				double ds = cs.getDistance() - s;
				double accel = (cs.getMaxVelocity() * cs.getMaxVelocity() - v * v) / (2.0 * ds);
				double dt = 0.0;
				if (i > 0)
				{
					timed[i - 1].setAcceleration(reverse ? -accel : accel);
					if (std::abs(accel) > kEpsilon)
					{
						dt = (cs.getMaxVelocity() - v) / accel;
					}
					else
					{
						dt = ds / v;
					}
				}
				t += dt;
				v = cs.getMaxVelocity();
				s = cs.getDistance();

				TimedTrajectoryPoint tpt(*cs.getState(), t, reverse ? -v : v, reverse ? -accel : accel);
				timed.push_back(tpt);
			}

			return TimedTrajectory(timed);
		}
	}
}