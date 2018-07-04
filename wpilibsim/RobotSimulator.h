#pragma once

#include <cstdint>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>

namespace frc
{
	class RobotSimulator
	{
	private:
		RobotSimulator();

	public:
		~RobotSimulator();

		static RobotSimulator &get();
		static void stop();

		double getYaw();
		void zeroYaw();
		void resetEncoder(int first, int second);
		int32_t getEncoder(int first, int second);
		void setMotor(int index, double v);

		void setLogFile(std::string &filename)
		{
			if (m_logfile.is_open())
				m_logfile.close();

			m_logfile.open(filename, std::ios::out);
			writeHeaders();
		}

		void setTimeInterval(double t) 
		{
			m_time_interval = t ;
		}

	private:
		void writeHeaders();
		void run();
		void calcPosition();
		void updateRobotPosition(double now, double dleft, double dright);

	private:
		static constexpr double PI = 3.141592;
		static RobotSimulator *m_robotsim_p;

		double m_last_time;
		double m_left_volts;
		double m_right_volts;
		double m_left_rps_per_volt_per_time;
		double m_right_rps_per_volt_per_time;

		int32_t m_ticks_per_rev;
		double m_diameter;
		double m_left;
		double m_right;
		double m_angle;
		double m_scrub;
		double m_width;

		bool m_waiting;
		bool m_running;
		std::mutex m_lock;
		std::thread m_run_thread;

		std::ofstream m_logfile;

		double m_last_output ;
		double m_time_interval ;
	};
}

