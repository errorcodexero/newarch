#include "SampleRobot.h"
#include "RobotSimBase.h"
#include <stdexcept>

using namespace xero::sim ;

namespace frc
{
	SampleRobot::SampleRobot()
	{
		m_enabled = false;
		m_mode = RobotMode::Autonomous;
		m_robotMainOverridden = false;
		m_start_delay = 0.25;
		m_auto_period = 30;
		m_teleop_period = 0;
		screen_ = false ;
	}

	SampleRobot::~SampleRobot()
	{
		m_controller.join();
	}

	void SampleRobot::robotSetup(SampleRobot *robot_p) {
		try {
			std::cout << "\r\n********** Robot program starting **********\r\n" << std::endl;
			robot_p->StartCompetition();
		}
		catch (const std::exception &ex)
		{
			std::cout << "\r\n******* Robot threw exception - " << ex.what() << " **************\r\n" << std::endl;
		}
	}

	void SampleRobot::InternalControl()
	{
		m_auto_done = false;
		double now ;
		std::chrono::microseconds delay(100) ;
		RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;				

		setEnabled(false);
		setRobotMode(SampleRobot::RobotMode::Autonomous);

		now = frc::Timer::GetFPGATimestamp() ;
		while (frc::Timer::GetFPGATimestamp() < now + m_start_delay)
			std::this_thread::sleep_for(delay) ;

		setEnabled(true);

		now = frc::Timer::GetFPGATimestamp() ;
		while (frc::Timer::GetFPGATimestamp() < now + m_auto_period && !m_auto_done)
			std::this_thread::sleep_for(delay) ;

		setEnabled(false);
		now = frc::Timer::GetFPGATimestamp() ;		
		while (frc::Timer::GetFPGATimestamp() < now + 1)
			std::this_thread::sleep_for(delay) ;

		setRobotMode(SampleRobot::RobotMode::Operator);
		setEnabled(true);

		now = frc::Timer::GetFPGATimestamp() ;
		while (frc::Timer::GetFPGATimestamp() < now + m_teleop_period)
			std::this_thread::sleep_for(delay) ;

		setRobotMode(SampleRobot::RobotMode::Finished) ;
		setEnabled(true);
		
		now = frc::Timer::GetFPGATimestamp() ;
		while (frc::Timer::GetFPGATimestamp() < now + 1)
			std::this_thread::sleep_for(delay) ;

		m_running = false;
	}

	bool SampleRobot::ParseDoubleArg(size_t index, double &value, const char *flag_p)
	{
		size_t end ;

		if (index == m_args.size()) {
			std::cout << "SampleRobot: argument '" << flag_p << "' requires an argument (real number)" << std::endl ;
			return false ;
		}

		try
		{
			value = std::stod(m_args[index], &end);
			if (end != m_args[index].length())
			{
				std::cout << "SampleRobot: invalid value '" << m_args[index];
				std::cout << "' for argument '" << flag_p << "'" << std::endl;
				return false ;
			}
		}
		catch (const std::exception &)
		{
			std::cout << "SampleRobot: invalid value '" << m_args[index];
			std::cout << "' for argument '" << flag_p << "'" << std::endl;
			return false ;
		}

		return true ;
	}

	bool SampleRobot::ProcessCmdLineArgs()
	{
		bool ret = true;
		size_t index = 1;

		while (index < m_args.size())
		{
			if (m_args[index] == "--start")
			{
				index++;
				if (index == m_args.size()) {
					std::cerr << "--start flag requires additional argument" << std::endl ;
					exit(1) ;
				}
				if (!ParseDoubleArg(index, m_start_delay, "--start"))
				{
					ret = false ;
					break ;
				}
				index++;
			}
			else if (m_args[index] == "--auto")
			{
				index++;
				if (index == m_args.size()) {
					std::cerr << "--auto flag requires additional argument" << std::endl ;
					exit(1) ;
				}				
				if (!ParseDoubleArg(index, m_auto_period, "--auto"))
				{
					ret = false ;
					break ;
				}
				index++;
			}	
			else if (m_args[index] == "--oper")
			{
				index++;
				if (index == m_args.size()) {
					std::cerr << "--oper flag requires additional argument" << std::endl ;
					exit(1) ;
				}					
				if (!ParseDoubleArg(index, m_teleop_period, "--oper"))
				{
					ret = false ;
					break ;
				}
				index++;
			}	
			else if (m_args[index] == "--speed")
			{
				double speed ;

				index++;
				if (index == m_args.size()) {
					std::cerr << "--speed flag requires additional argument" << std::endl ;
					exit(1) ;
				}					
				if (!ParseDoubleArg(index, speed, "--speed"))
				{
					ret = false ;
					break ;
				}

				if (speed < 0.5 || speed > 100.0) {
					std::cerr << "invalid value for --speed flag, expected 0.5 to 100.0" << std::endl ;
					std::cerr << "   value parsed was " << speed ;
					exit(1) ;
				}

				RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
				sim.setSpeed(speed) ;				
				index++;
			}				
			else if (m_args[index] == "--simprint") {
				RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;				
				sim.enablePrinting() ;
				index++ ;
			}
			else if (m_args[index] == "--simfile") {
				index++;
				if (index == m_args.size()) {
					std::cerr << "--simfile flag requires additional argument" << std::endl ;
					exit(1) ;
				}					
				RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
				sim.enablePrinting(m_args[index]) ;
				index++ ;
			}
			else if (m_args[index] == "--simscreen") {
				RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;				
				sim.enableScreen() ;
				index++ ;
				screen_ = true ;
			}
			else if (m_args[index] == "--simprop") {
				index++ ;
				if (index == m_args.size()) {
					std::cerr << "--simprop flag requires additional argument" << std::endl ;
					exit(1) ;
				}					
				RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
				if (!sim.setProperty(m_args[index])) {
					std::cerr << "--simprop flag, additional argument was invalid" << std::endl ;
					exit(1) ;					
				}
				index++ ;				
			}
			else
			{
				std::cout << "SampleRobot: invalid command line argument '";
				std::cout << m_args[index] << "' - robot aborting" << std::endl;
				std::cout << "Valid Arguments" << std::endl;
				std::cout << "  --start NUMBER" << std::endl;
				std::cout << "  --auto NUMBER" << std::endl;
				std::cout << "  --oper NUMBER" << std::endl;
				std::cout << "  --simprint" << std::endl ;
				std::cout << "  --simfile file" << std::endl ;
				std::cout << "  --simscreen" << std::endl ;								
				std::cout << "  --simprop property" << std::endl ;
				ret = false;
				break;
			}
		}

		return ret;
	}


	void SampleRobot::StartCompetition()
	{
		//
		// Process command line arguments
		//
		if (!ProcessCmdLineArgs())
			return;

		//
		// Start the robot simulator
		//
		RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
		sim.start() ;

		//
		// Initialize the robot hardware
		//
		RobotInit();

		//
		// Allow a derived class to take over the main loop
		//
		RobotMain();

		//
		// Start the thread that manages the competition
		//
		m_controller = std::thread(&SampleRobot::InternalControl, this);

		//
		// If the main loop was not taken over, supply a main loop where
		// we ask the robot to handle specific modes
		//
		if (!m_robotMainOverridden) {
			bool first = true ;
			m_running = true;
			while (m_running) {
				if (IsDisabled()) {
					Disabled();
					while (IsDisabled() && !IsFinished());
				}
				else if (IsAutonomous()) {
					Autonomous();
					m_auto_done = true;
					while (IsAutonomous() && IsEnabled());
				}
				else if (IsTest()) {
					Test();
					while (IsTest() && IsEnabled());
				}
				else if (IsOperatorControl()) {
					OperatorControl();
					while (IsOperatorControl() && IsEnabled());
				}
				else if (m_mode == RobotMode::Finished) {
					if (first) {
						first = false ;
					}
				}
				else {
					std::cout << "BadState" << std::endl ;
				}
			}

			sim.stop() ;			
		}
	}


	bool SampleRobot::IsDisabled() const 
	{
		return !m_enabled;
	}

	bool SampleRobot::IsFinished() const
	{
		return m_mode == RobotMode::Finished;
	}

	bool SampleRobot::IsAutonomous() const 
	{
		return m_mode == RobotMode::Autonomous;
	}

	bool SampleRobot::IsEnabled() const 
	{
		return m_enabled;
	}

	bool SampleRobot::IsTest() const 
	{
		return m_mode == RobotMode::Test;
	}

	bool SampleRobot::IsOperatorControl() const 
	{
		return m_mode == RobotMode::Operator;
	}
}
