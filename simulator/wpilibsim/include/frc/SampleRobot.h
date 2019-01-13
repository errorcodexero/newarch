#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <cassert>

namespace frc
{
    template <class Robot> int StartRobot() {
        static Robot robot;
        robot.StartCompetition();
        return 0;
    }

    class SampleRobot
    {
    public:
        SampleRobot();
        virtual ~SampleRobot();

        void addArg(const char *arg_p)
        {
            m_args.push_back(arg_p);
        }

        static void robotSetup(SampleRobot *robot_p);

        bool IsEnabled() const;
        bool IsFinished() const;
        bool IsDisabled() const;
        bool IsAutonomous() const;
        bool IsTest() const;
        bool IsOperatorControl() const;

        void StartCompetition();

        virtual void RobotInit()
        {
        }

        virtual void Disabled()
        {
        }

        virtual void Autonomous()
        {
        }

        virtual void OperatorControl()
        {
        }

        virtual void Test()
        {
        }

        virtual void RobotMain()
        {
        }

    protected:
        void setEnabled(bool en)
        {
            m_enabled = en;
        }

        bool isScreen() const {
            return screen_ ;
        }


    private:
        bool ParseDoubleArg(size_t index, double &value, const char *flag_p) ;
        bool ProcessCmdLineArgs();
        void InternalControl();

    protected:
        enum class RobotMode
        {
            Autonomous,
            Operator,
            Test,
            Finished,
        };

        void setRobotMode(SampleRobot::RobotMode m)
        {
            m_mode = m;
        }

        const std::string getRobotOutputFile() {
            return output_file_name_ ;
        }

    private:
        bool m_running;
        bool m_enabled;
        bool m_robotMainOverridden;
        RobotMode m_mode;
        std::vector<std::string> m_args;
        std::thread m_controller;

        double m_start_delay;
        double m_auto_period;
        double m_teleop_period;
        double m_test_period ;

        bool m_auto_done;
        bool screen_ ;

        std::string output_file_name_ ;
    };
}

