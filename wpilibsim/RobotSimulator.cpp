#include "RobotSimulator.h"
#include "Timer.h"
#include <cassert>
#include <iostream>

namespace frc
{
	RobotSimulator *RobotSimulator::m_robotsim_p = nullptr;

	RobotSimulator::RobotSimulator()
	{
		m_left = 0.0;
		m_right = 0.0;
		m_angle = 0.0;
		m_ticks_per_rev = 200;
		m_diameter = 5.0;
		m_last_time = 0.0;
		m_left_volts = 0.0;
		m_right_volts = 0.0;
		m_scrub = 1.0;
		m_width = 24.0;
		m_right_rps_per_volt_per_time = 9.16;
		m_left_rps_per_volt_per_time = 9.16;

		m_time_interval = 100000.0 ;
		m_last_output = 0.0 ;

		m_running = true;
		m_run_thread = std::thread(&RobotSimulator::run, this);
	}

	RobotSimulator::~RobotSimulator()
	{
		m_waiting = true;
		m_running = false;
		m_run_thread.join();
	}

	RobotSimulator &RobotSimulator::get()
	{
		if (m_robotsim_p == nullptr)
			m_robotsim_p = new RobotSimulator();

		return *m_robotsim_p;
	}

	void RobotSimulator::stop()
	{
		delete m_robotsim_p;
		m_robotsim_p = nullptr;
	}

	void RobotSimulator::zeroYaw()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_angle = 0.0;
	}

	double RobotSimulator::getYaw()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		return -m_angle / PI * 180 ;
	}

	void RobotSimulator::setMotor(int index, double v)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		if (index == 0)
			m_left_volts = v;
		else if (index == 1)
			m_right_volts = v;
		else
			assert(false);

	}

	void RobotSimulator::resetEncoder(int first, int second)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		if (first == 0 && second == 1)
		{
			m_left = 0.0;
		}
		else if (first == 2 && second == 3)
		{
			m_right = 0.0;
		}
		else
		{
			assert(false);
		}
	}

	int32_t RobotSimulator::getEncoder(int first, int second)
	{
		double dist;

		std::lock_guard<std::mutex> lock(m_lock);
		if (first == 0 && second == 1)
		{
			dist = m_left;
		}
		else if (first == 2 && second == 3)
		{
			dist = m_right;
		}
		else
		{
			assert(false);
		}

		double revs = dist / (PI * m_diameter);
		return static_cast<int32_t>(revs * m_ticks_per_rev);
	}

	void RobotSimulator::run()
	{
		std::chrono::microseconds delay(50);
		while (m_running)
		{
			calcPosition();
			std::this_thread::sleep_for(delay);
		}

		m_waiting = false;
	}

	void RobotSimulator::updateRobotPosition(double now, double dleft, double dright)
	{
		m_left += dleft;
		m_right += dright;

		double dv = (dright - dleft) / 2 * m_scrub;
		m_angle += (dv * 2.0) / m_width;

		m_logfile << now;
		m_logfile << "," << m_left_volts;
		m_logfile << "," << m_right_volts;
		m_logfile << "," << m_left;
		m_logfile << "," << m_right;
		m_logfile << "," << m_angle;
		m_logfile << std::endl;
	}

	void RobotSimulator::writeHeaders()
	{
		m_logfile << "t";
		m_logfile << ",leftv";
		m_logfile << ",rightv";
		m_logfile << ",leftdist";
		m_logfile << ",rightdist";
		m_logfile << ",angle";
		m_logfile << std::endl;
	}

	void RobotSimulator::calcPosition()
	{
		std::lock_guard<std::mutex> lock(m_lock);

		//
		// We know the left and right motors have been running at a 
		// given voltage since the m_last_time_value 
		//
		double now = frc::Timer::GetFPGATimestamp();
		double elapsed = now - m_last_time;
		double dleft = m_left_volts * m_left_rps_per_volt_per_time * elapsed * m_diameter * PI;
		double dright = m_right_volts * m_right_rps_per_volt_per_time * elapsed * m_diameter * PI;

		updateRobotPosition(now, dleft, dright);
		m_last_time = now;

		if (now > m_last_output + m_time_interval) {
			std::cout << "Robot Status: time " << now ;
			std::cout << ", leftpos " << m_left ;
			std::cout << ", rightpos " << m_right ;
			std::cout << ", angle " << m_angle ;
			std::cout << std::endl ;
			m_last_output = now ;
		}
	}
}
