#include "SampleRobot.h"
#include <networktables/NetworkTableInstance.h>
#include <chrono>
#include <iostream>
#include <csignal>
#include <pthread.h>
#include <sys/syscall.h>

namespace frc
{

    SampleRobot *SampleRobot::theone = nullptr;

    void SampleRobot::ControlCHandler(int sig)
    {
        std::cout << "Control-C detected, shutting down robot" << std::endl;
        SampleRobot &robot = SampleRobot::GetInstance();

        nt::NetworkTableInstance::GetDefault().StopServer();

        robot.m_driver_station_run = false;
        robot.setRobotMode(RobotBase::RobotMode::Finished);

        std::cout << "    Shuting down robot outputs" << std::endl;
        robot.stopAll(false);
        std::cout << "    Exit From Robot Program" << std::endl;
        _exit(0);
    }

    SampleRobot::SampleRobot() {
        assert(theone == nullptr);

        theone = this;
        m_robotMainOverridden = true;
        m_start_delay = 1.0;
        m_auto_period = 15.0;
        m_teleop_period = 0.0;

        nt::NetworkTableInstance::GetDefault().StartServer();
    }

    SampleRobot::~SampleRobot() {
        theone = nullptr;
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

#ifdef DEBUG_PRINT_THREAD_ID
        int pid = syscall(SYS_gettid);
        std::cout << "Internal controller thread id " << pid << std::endl;
#endif

        //
        // This ensures we mimic field behavior a little.  The robot is
        // ensured to be in the disabled state for at least 500 milliseconds
        // before moving into any other state.  Any work that is to be done
        // while in the disabled state will happen here.
        //
        if (m_start_delay < 0.5)
            m_start_delay = 0.5 ;

        setEnabled(false);
        setRobotMode(RobotBase::RobotMode::Autonomous);
        ms = static_cast<int>(m_start_delay * 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));

        if (getRobotMode() == RobotBase::RobotMode::Finished) {
            std::cout << "DEBUG: skipping out of internal control" << std::endl;
            return;
        }

        setEnabled(true);
        ms = static_cast<int>(m_auto_period * 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));

        if (getRobotMode() == RobotBase::RobotMode::Finished)
        {
            std::cout << "DEBUG: skipping out of internal control" << std::endl;
            return;
        }

        setEnabled(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        setRobotMode(RobotBase::RobotMode::Operator);
        setEnabled(true);

        if (getRobotMode() == RobotBase::RobotMode::Finished)
        {
            std::cout << "DEBUG: skipping out of internal control" << std::endl;
            return;
        }

        ms = static_cast<int>(m_teleop_period * 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));

        setEnabled(false);
        stopAll();

        m_running = false;
        setRobotMode(RobotBase::RobotMode::Finished);
    }

    void SampleRobot::DriverStationControl()
    {
#ifdef DEBUG_PRINT_THREAD_ID
        int pid = syscall(SYS_gettid);
        std::cout << "Driver station  thread id " << pid << std::endl;
#endif

        DriverStation &st = DriverStation::GetInstance();
        setRobotMode(RobotBase::RobotMode::Operator);
        setEnabled(false);



        m_driver_station_run = true;

        while (m_driver_station_run)
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

        std::cout << "DEBUG: exiting down driver station control thread" << std::endl;
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
