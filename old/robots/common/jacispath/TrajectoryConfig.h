#pragma once

namespace xero
{
	namespace jacispath
	{
		class TrajectoryConfig
		{
		public:
			TrajectoryConfig()
			{
				m_dt = 0.0;
				m_max_vel = 0.0;
				m_max_accel = 0.0;
				m_max_jerk = 0.0;
				m_src_vel = 0.0;
				m_src_theta = 0.0;
				m_dest_pos = 0.0;
				m_dest_vel = 0.0;
				m_dest_theta = 0.0;
				m_count = 0;
			}

			TrajectoryConfig(double dt, double maxv, double maxa, double maxj, double srcv, double srctheta, 
				double destpos, double destvel, double desttheta, size_t count)
			{
				m_dt = dt;
				m_max_vel = maxv;
				m_max_accel = maxa;
				m_max_jerk = maxj;
				m_src_vel = srcv;
				m_src_theta = srctheta;
				m_dest_pos = destpos;
				m_dest_vel = destvel;
				m_dest_theta = desttheta;
				m_count = count;
			}

			virtual ~TrajectoryConfig();

			size_t getCount() const
			{
				return m_count;
			}

			double getDeltaTime() const
			{
				return m_dt;
			}

			double getMaxVel() const
			{
				return m_max_vel;
			}

			double getMaxAccel() const
			{
				return m_max_accel;
			}

			double getMaxJerk() const
			{
				return m_max_jerk;
			}

			double getSrcVel() const
			{
				return m_src_vel;
			}

			double getSrcTheta() const
			{
				return m_src_theta;
			}

			double getDestPos() const
			{
				return m_dest_pos;
			}

			double getDestVel() const
			{
				return m_dest_vel;
			}

			double getDestTheta() const
			{
				return m_dest_theta;
			}

		private:
			double m_dt;
			double m_max_vel;
			double m_max_accel;
			double m_max_jerk;
			double m_src_vel;
			double m_src_theta;
			double m_dest_pos;
			double m_dest_vel;
			double m_dest_theta;
			size_t m_count;
		};
	}
}

