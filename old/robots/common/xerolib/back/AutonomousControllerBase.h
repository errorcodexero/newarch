#pragma once

#include "RobotControllerBase.h"
#include <memory>
#include <vector>

namespace xerolib
{
	class XeroRobotBase;
	class AutonomousStepBase;

	class AutonomousControllerBase : public RobotControllerBase
	{
	public:
		AutonomousControllerBase(XeroRobotBase &robot);
		~AutonomousControllerBase();

		virtual bool isRunning();
		virtual void executeOneLoop();

	protected:
		void addStep(std::shared_ptr<AutonomousStepBase> step_p)
		{
			m_steps.push_back(step_p);
		}

		virtual void onStartAutonomousProgram()
		{
		}

		virtual void onFinishAutonomousProgram()
		{
		}


	private:
		void nextStep(double now);

		enum class State
		{
			Beginning,
			Running,
			End
		};

	private:
		//
		// The current state of the autonomous controller
		//
		State m_state;

		//
		// The list of steps to execute
		//
		std::vector<std::shared_ptr<AutonomousStepBase>> m_steps;

		//
		// The current step of autonomous we are on
		//
		size_t m_current_step;
	};
}

