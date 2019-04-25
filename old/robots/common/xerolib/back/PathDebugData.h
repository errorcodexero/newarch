#pragma once

namespace xero
{
	namespace pathfinder
	{
		class PathDebugData
		{
		public:
			PathDebugData()
			{
			}

			~PathDebugData()
			{
			}

			double t;
			size_t m_segment_index;
			double m_segment_remaining;
			double pose_x;
			double pose_y;
			double pose_theta;
			double linear_displacement;
			double linear_velocity;
			double profile_displacement;
			double profile_velocity;
			double velocity_command_dx;
			double velocity_command_dy;
			double velocity_command_dtheta;
			double steering_command_dx;
			double steering_command_dy;
			double steering_command_dtheta;
			double cross_track_error;
			double along_track_error;
			double lookahead_point_x;
			double lookahead_point_y;
			double lookahead_point_velocity;
			double arc_center_x;
			double arc_center_y;
			double arc_radius;
			double arc_length;
			double curvature;
			double dtheta;
			double left;
			double right;
			double m_closest_x;
			double m_closest_y;
			double m_seg_start_x;
			double m_seg_start_y;
			double m_seg_end_x;
			double m_seg_end_y;
		};
	}
}

