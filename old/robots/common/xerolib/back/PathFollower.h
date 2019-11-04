#pragma once

#include "Lookahead.h"
#include "AdaptivePurePursuitController.h"
#include "ProfileFollower.h"

namespace xero
{
	namespace pathfinder
	{
		class PathFollower
		{
		public:
			class Parameters
			{
			public:
				Lookahead lookahead;
				double inertia_gain;
				double profile_kp;
				double profile_ki;
				double profile_kv;
				double profile_kffv;
				double profile_kffa;
				double profile_max_abs_vel;
				double profile_max_abs_acc;
				double goal_pos_tolerance;
				double goal_vel_tolerance;
				double stop_steering_distance;

				Parameters(const Lookahead &la, double igain, double kp, double ki, double kv, double kffv, double kffa,
					double max_abs_vel, double max_abs_acc, double pos_tol, double vel_tol, double stop_st_dist) : lookahead(la)
				{
					inertia_gain = igain;
					profile_kp = kp;
					profile_ki = ki;
					profile_kv = kv;
					profile_kffv = kffv;
					profile_kffa = kffa;
					profile_max_abs_vel = max_abs_vel;
					profile_max_abs_acc = max_abs_acc;
					goal_pos_tolerance = pos_tol;
					goal_vel_tolerance = vel_tol;
					stop_steering_distance = stop_st_dist;
				}
			};

		public:
			PathFollower(std::shared_ptr<Path> path_p, bool reversed, const Parameters &params);
			virtual ~PathFollower();

			xero::math::PositionAngle update(double t, const xero::math::PositionCS &pose, double disp, double vel, xero::pathfinder::PathDebugData &debug);

			double getCrossTrackError() const
			{
				return m_cross_track_error;
			}

			double getAlongTrackError() const
			{
				return m_along_track_error;
			}

			bool isFinished() const
			{
				if (m_override_finished)
					return true;

				return m_steering_controller_p->isFinished() && m_velocity_controller_p->isFinishedProfile() && m_velocity_controller_p->onTarget();
			}

			void forceFinish()
			{
				m_override_finished = true;
			}

			bool hasPassedMarker(const char *marker_p) const
			{
				return m_steering_controller_p->hasPassedMarker(marker_p);
			}

		private:
			std::shared_ptr<xero::motion::ProfileFollower> m_velocity_controller_p;
			std::shared_ptr<AdaptivePurePursuitController> m_steering_controller_p;
			xero::math::PositionAngle m_last_steering_delta;
			double m_inertia_gain;
			bool m_override_finished;
			bool m_done_steering;

			double m_max_profile_vel;
			double m_max_profile_acc;
			double m_goal_pos_tolerance;
			double m_goal_vel_tolerance;
			double m_stop_steering_distance;
			double m_cross_track_error;
			double m_along_track_error;
		};
	}
}
