#include "XeroRobotBase.h"
#include "RobotControllerBase.h"
#include "SubsystemBase.h"
#include "Timer.h"
#include "ParamsParser.h"
#include "ModuleDefintions.h"
#include <iostream>

namespace xerolib
{
    XeroRobotBase::XeroRobotBase()
    {
		m_looptime = 0.02;
    }

    XeroRobotBase::~XeroRobotBase()
    {
    }

	void XeroRobotBase::RobotInit()
	{
		MessageLogger &logger = getMessageLogger();
		logger.enableModules(MODULE_ALL);
		setupConsoleLogger();
		setupFileLogger(getBaseDir() + "\\robot.log");

		ParamsParser &params = ParamsParser::get();
		params.readFile(getBaseDir() + "\\params.txt");

		m_looptime = params.getValue("robot:looptime", 0.02);
	}

    void XeroRobotBase::setupConsoleLogger()
    {
		m_logger.addStandardOutputDestination();
    }

	void XeroRobotBase::setupFileLogger(const char *filename_p)
	{
		m_logger.addDestination(filename_p);
	}

    void XeroRobotBase::Disabled()
    {
		auto &log = getMessageLogger();
		log.startMessage(MessageLogger::MessageType::Info, MODULE_ROBOTBASE);
		log << "Entering mode: Disabled";
		log.endMessage();
    }

    void XeroRobotBase::Autonomous()
    {
		auto &log = getMessageLogger();
		log.startMessage(MessageLogger::MessageType::Info, MODULE_ROBOTBASE);
		log << "Entering mode: Autonomous";
		log.endMessage();

		m_controller_p = createAutonomousController();

		while (IsAutonomous() && IsEnabled() && m_controller_p->isRunning())
		{
			doOneLoop(RobotModeType::AutonomousMode);
		}
    }

    void XeroRobotBase::OperatorControl()
    {
		auto &log = getMessageLogger();
		log.startMessage(MessageLogger::MessageType::Info, MODULE_ROBOTBASE);
		log << "Entering mode: OperatorControl";
		log.endMessage();

		m_controller_p = createOperatorControlController();

		while (IsOperatorControl() && IsEnabled())
		{
			doOneLoop(RobotModeType::OperatorMode);
		}
    }

    void XeroRobotBase::Test()
    {
		auto &log = getMessageLogger();
		log.startMessage(MessageLogger::MessageType::Info, MODULE_ROBOTBASE);
		log << "Entering mode: Test";
		log.endMessage();

		m_controller_p = createTestController();

		while (IsTest() && IsEnabled())
		{
			doOneLoop(RobotModeType::TestMode);
		}
    }

    void XeroRobotBase::doOneLoop(RobotModeType mode)
    {
		double start = getTime();

		for (auto sub_p : m_subsystems)
			sub_p->getInputs();

		if (m_controller_p != nullptr)
			m_controller_p->executeOneLoop();

		for (auto sub_p : m_subsystems)
			sub_p->setOutputs();

		double elapsed = getTime() - start;

		if (elapsed < m_looptime)
		{
			frc::Wait(m_looptime - elapsed);
		}
		else
		{
			auto &log = getMessageLogger();
			log.startMessage(MessageLogger::MessageType::Warning, MODULE_ROBOTBASE);
			log << "robot exceeded the target loop time, target " << m_looptime;
			log << ", actual " << elapsed;
			log.endMessage();
		}
    }

    std::shared_ptr<SubsystemBase> XeroRobotBase::getSubsystem(const char *name_p)
    {
		for (auto sub_p : m_subsystems)
		{
			if (sub_p->getName() == name_p)
				return sub_p;
		}

		return nullptr;
    }
}
