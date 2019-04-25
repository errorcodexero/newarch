#include "AutonomousControllerBase.h"
#include "AutonomousStepBase.h"
#include "XeroRobotBase.h"
#include "ModuleDefintions.h"
#include <iostream>

namespace xero
{
	namespace base
	{
		AutonomousControllerBase::AutonomousControllerBase(XeroRobotBase &robot) : RobotControllerBase(robot)
		{
			m_state = State::Beginning;
		}

		AutonomousControllerBase::~AutonomousControllerBase()
		{
		}

		bool AutonomousControllerBase::isRunning()
		{
			return m_state != State::End;
		}

		void AutonomousControllerBase::nextStep(double now)
		{
			MessageLogger &logger = getRobot().getMessageLogger();
			std::shared_ptr<AutonomousStepBase> step_p = m_steps[m_current_step];

			//
			// if run() returns true, the step is complete
			//
			logger.startMessage(MessageLogger::MessageType::Debug, MODULE_AUTOMODE);
			logger << now << ": Ending step '" << step_p->toString() << "'";
			logger.endMessage();

			step_p->end();
			m_current_step++;

			if (m_current_step == m_steps.size())
			{
				m_state = State::End;
				onFinishAutonomousProgram();
			}
			else
			{
				step_p = m_steps[m_current_step];
				logger.startMessage(MessageLogger::MessageType::Debug, MODULE_AUTOMODE);
				logger << now << ": Starting step '" << step_p->toString() << "'";
				logger.endMessage();
				step_p->start();
			}
		}

		void AutonomousControllerBase::executeOneLoop()
		{
			double now = getRobot().getTime();
			MessageLogger &logger = getRobot().getMessageLogger();
			bool running = true;
			std::shared_ptr<AutonomousStepBase> step_p;

			while (running && m_state != State::End)
			{
				if (m_state == State::Beginning)
				{
					logger.startMessage(MessageLogger::MessageType::Debug, MODULE_AUTOMODE);
					logger << now << ": Starting autonomous program";
					logger.endMessage();
					onStartAutonomousProgram();

					//
					// This is the first time we have been run.  If there
					// are no steps, jump to the end.  Otherwise, we start
					// the first step.
					//
					if (m_steps.size() == 0)
					{
						m_state = State::End;
						running = false;
						onFinishAutonomousProgram();
					}
					else
					{
						m_current_step = 0;
						step_p = m_steps[m_current_step];
						logger.startMessage(MessageLogger::MessageType::Debug, MODULE_AUTOMODE);
						logger << now << ": Starting step '" << step_p->toString() << "'";
						logger.endMessage();
						step_p->start();

						m_state = State::Running;
						if (step_p->run())
							nextStep(now);
						else
							running = false;
					}
				}
				else if (m_state == State::Running)
				{
					step_p = m_steps[m_current_step];
					if (step_p->run())
						nextStep(now);
					else
						running = false;
				}
			}
		}
	}
}