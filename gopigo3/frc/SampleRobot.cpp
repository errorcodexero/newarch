#include "SampleRobot.h"
#include <networktables/NetworkTableInstance.h>
#include <chrono>
#include <iostream>
#include <csignal>

namespace frc
{

    SampleRobot *SampleRobot::theone = nullptr;

    void SampleRobot::ControlCHandler(int sig)
    {
        std::cout << "Control-C detected, shutting down robot" << std::endl;
        SampleRobot &robot = SampleRobot::GetInstance();
        std::cout << "    Got robot instance - shutting down motors" << std::endl;
        robot.stopAll();
        std::cout << "    Shut down motors - performaing a hard exit" << std::endl;
        exit(0);
        std::cout << "    Should never get here" << std::endl;
    }

    SampleRobot::SampleRobot() {
		m_robotMainOverridden = true;
		m_start_delay = 1.0;
		m_auto_period = 15.0;
		m_teleop_period = 0.0;

        nt::NetworkTableInstance::GetDefault().StartServer();
    }

    SampleRobot::~SampleRobot() {
	}

   

	void SampleRobot::RobotMain() {
		m_robotMainOverridden = false;
	}

	void SampleRobot::RobotInit() {
	}

	void SampleRobot::Disabled() {
		std::cout << "Robot Mode - disabled" << std::endl;
	}

	void SampleRobot::Autonomous() {
		std::cout << "Robot Mode - autonomous" << std::endl;
	}

	void SampleRobot::Test() {
	}

	void SampleRobot::OperatorControl() {
	}

	void SampleRobot::InternalControl()
	{
		int ms;

		if (m_start_delay < 0.5)
			m_start_delay = 0.5 ;

		setEnabled(false);
		setRobotMode(RobotBase::RobotMode::Autonomous);
		ms = static_cast<int>(m_start_delay * 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		setEnabled(true);
		ms = static_cast<int>(m_auto_period * 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		setEnabled(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		setRobotMode(RobotBase::RobotMode::Operator);
		setEnabled(true);

		ms = static_cast<int>(m_teleop_period * 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		setEnabled(false);
		stopAll();

		m_running = false;
		setRobotMode(RobotBase::RobotMode::Finished);
	}

	void SampleRobot::DriverStationControl()
	{
		DriverStation &st = DriverStation::GetInstance();
		setRobotMode(RobotBase::RobotMode::Operator);
		setEnabled(false);

		while (true)
		{
			if (st.IsTest() && getRobotMode() != RobotMode::Test)
				setRobotMode(RobotMode::Test);
			else if (st.IsAutonomous() && getRobotMode() != RobotMode::Autonomous)
				setRobotMode(RobotMode::Autonomous);
			else if (st.IsOperatorControl() && getRobotMode() != RobotMode::Operator)
				setRobotMode(RobotMode::Operator);

			if (st.IsEnabled() && !IsEnabled())
				setEnabled(true);
			else if (st.IsDisabled() && !IsDisabled())
				setEnabled(false);

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	bool SampleRobot::ProcessCmdLineArgs()
	{
		bool ret = true;
		const std::vector<std::string> &args = getArgs();
		size_t index = 1;

		while (index < args.size())
		{
			if (args[index] == "--start" || args[index] == "--auto" || args[index] == "--oper")
			{
				index++;
				if (index == args.size())
				{
					std::cout << "SampleRobot: argument '--start' requires an argument" << std::endl;
					ret = false;
					break;
				}

				double value;
				size_t end;

				try
				{
					value = std::stod(args[index], &end);
					if (end != args[index].length())
					{
						std::cout << "SampleRobot: invalid value '" << args[index];
						std::cout << "' for argument '" << args[index - 1] << "'" << std::endl;
						ret = false;
						break;
					}

					if (args[index - 1] == "--start") {
						m_start_delay = value;
					}
					else if (args[index - 1] == "--auto")
						m_auto_period = value;
					else if (args[index - 1] == "--oper")
						m_teleop_period = value;
				}
				catch (const std::exception &ex)
				{
					std::cout << "SampleRobot: invalid value '" << args[index];
					std::cout << "' for argument '" << args[index - 1] << "'" << std::endl;
					ret = false;
					break;
				}

				index++;
			}
			else if (args[index] == "--station")
			{
				m_station = true;
				index++;
			}
			else
			{
				std::cout << "SampleRobot: invalid command line argument '";
				std::cout << args[index] << "' - robot aborting" << std::endl;
				std::cout << "Valid Arguments" << std::endl;
				std::cout << "  --start NUMBER" << std::endl;
				std::cout << "  --auto NUMBER" << std::endl;
				std::cout << "  --oper NUMBER" << std::endl;
				ret = false;
				break;
			}
		}

		return ret;
	}

	void SampleRobot::StartCompetition() 
	{
        signal(SIGINT, SampleRobot::ControlCHandler);

        //
		// Process command line arguments
		//
		if (!ProcessCmdLineArgs())
			return;

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
		if (m_station)
			m_controller = std::thread(&SampleRobot::DriverStationControl, this);
		else
			m_controller = std::thread(&SampleRobot::InternalControl, this);

		//
		// If the main loop was not taken over, supply a main loop where
		// we ask the robot to handle specific modes
		//
		if (!m_robotMainOverridden) {
			m_running = true;
			while (m_running) {
				if (IsDisabled()) {
					Disabled();
					while (IsDisabled());
				}
				else if (IsAutonomous()) {
					Autonomous();
					while (IsAutonomous() && IsEnabled());
				}
				else if (IsTest()) {
					Test();
					while (IsTest() && IsEnabled());
				}
				else {
					OperatorControl();
					while (IsOperatorControl() && IsEnabled());
				}
			}

			std::cout << "\r\n ************ Robot program ending ***********" << std::endl;
		}
	}
}
