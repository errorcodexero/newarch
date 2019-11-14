#include <frc/SampleRobot.h>
#include <frc/RobotSimBase.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <stdexcept>

using namespace xero::sim ;
using namespace xero::misc ;

namespace frc
{
    SampleRobot::SampleRobot()
    {
        m_enabled = false;
        m_mode = RobotMode::Autonomous;
        m_robotMainOverridden = false;
        m_start_delay = 0;
        m_auto_period = 0;
        m_teleop_period = 0;
        m_test_period = 120 ;
        screen_ = false ;

        nt::NetworkTableInstance::GetDefault().StartServer() ;
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

        std::cout << "InternalControl: started" << std::endl;

        if (m_test_period > 0.0) {
            setRobotMode(SampleRobot::RobotMode::Test) ;
            setEnabled(true) ;

            now = frc::Timer::GetFPGATimestamp() ;
            while (frc::Timer::GetFPGATimestamp() < now + m_test_period)
                std::this_thread::sleep_for(delay) ;            
        }
        else {
            setEnabled(false);
            setRobotMode(SampleRobot::RobotMode::Autonomous);

            std::cout << "InternalControl: waiting to start autonomous" << std::endl;
            now = frc::Timer::GetFPGATimestamp() ;
            while (frc::Timer::GetFPGATimestamp() < now + m_start_delay)
                std::this_thread::sleep_for(delay) ;

            std::cout << "InternalControl: starting autonomous" << std::endl;
            setEnabled(true);

            now = frc::Timer::GetFPGATimestamp() ;
            while (frc::Timer::GetFPGATimestamp() < now + m_auto_period && !m_auto_done)
                std::this_thread::sleep_for(delay) ;

            std::cout << "InternalControl: stopping autonomous" << std::endl;
            setEnabled(false);
            now = frc::Timer::GetFPGATimestamp() ;      
            while (frc::Timer::GetFPGATimestamp() < now + 1)
                std::this_thread::sleep_for(delay) ;
            
            std::cout << "InternalControl: starting teleop" << std::endl;
            setRobotMode(SampleRobot::RobotMode::Operator);
            setEnabled(true);


            now = frc::Timer::GetFPGATimestamp() ;
            while (frc::Timer::GetFPGATimestamp() < now + m_teleop_period)
                std::this_thread::sleep_for(delay) ;
        }

        std::cout << "InternalControl: finishing" << std::endl;

        setRobotMode(SampleRobot::RobotMode::Finished) ;
        setEnabled(true);
        
        now = frc::Timer::GetFPGATimestamp() ;
        while (frc::Timer::GetFPGATimestamp() < now + 1)
            std::this_thread::sleep_for(delay) ;

        m_running = false;

        std::cout << "InternalControl: done" << std::endl;
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
            if (m_args[index] == "--events")
            {
                index++ ;
                if (index == m_args.size()) {
                    std::cerr << "--events flag requires additional argument" << std::endl ;
                    exit(1) ;
                }   

                RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
                if (!sim.readEvents(m_args[index++])) {
                    std::cerr << "--events provide file name is not valid" << std::endl ;
                    exit(1) ;
                }
            }
            else if (m_args[index] == "--start")
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

                if (m_start_delay < 1.0)
                    m_start_delay = 1.0 ;
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
            else if (m_args[index] == "--test")
            {
                index++;
                if (index == m_args.size()) {
                    std::cerr << "--test flag requires additional argument" << std::endl ;
                    exit(1) ;
                }                   
                if (!ParseDoubleArg(index, m_test_period, "--test"))
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
            else if (m_args[index] == "--robotfile") {
                index++;
                if (index == m_args.size()) {
                    std::cerr << "--robotfile flag requires additional argument" << std::endl ;
                    exit(1) ;
                }                   
                output_file_name_ = m_args[index] ;
                index++ ;
            }
            else if (m_args[index] == "--simscreen") {
                RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;             
                sim.enableScreen() ;
                index++ ;
                screen_ = true ;
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
        sim.start(this) ;

        std::cout << "StartCompetition: RobotInit" << std::endl;
        //
        // Initialize the robot hardware
        //
        RobotInit();
        
        std::cout << "StartCompetition: RobotMain" << std::endl;
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
            std::cout << "StartCompetition: !m_RobotMainOverridden" << std::endl;
            bool first = true ;
            m_running = true;
            while (m_running) {
                if (IsDisabled()) {
                    std::cout << "StartCompetition: disabled" << std::endl;
                    Disabled();
                    while (IsDisabled() && !IsFinished());
                }
                else if (IsAutonomous()) {
                    std::cout << "StartCompetition: autonomous" << std::endl;
                    Autonomous();
                    m_auto_done = true;
                    while (IsAutonomous() && IsEnabled());
                }
                else if (IsTest()) {
                    std::cout << "StartCompetition: test" << std::endl;
                    Test();
                    while (IsTest() && IsEnabled());
                }
                else if (IsOperatorControl()) {
                    std::cout << "StartCompetition: teleop" << std::endl;
                    OperatorControl();
                    while (IsOperatorControl() && IsEnabled());
                }
                else if (m_mode == RobotMode::Finished) {
                    if (first) {
                        std::cout << "StartCompetition: finished" << std::endl;
                        first = false ;
                    }
                }
                else {
                    std::cout << "BadState" << std::endl ;
                }
            }
            
            std::cout << "StartCompetition: done" << std::endl;
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
