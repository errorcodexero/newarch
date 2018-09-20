#include "RobotBase.h"
#include <iostream>
#include <cassert>

namespace frc {

	RobotBase *RobotBase::m_robot_base_p = nullptr;

	RobotBase::RobotBase() {
		assert(m_robot_base_p == nullptr);
		m_robot_base_p = this;

		setRobotMode(RobotMode::Operator);
		setEnabled(false);
		m_hardware.init();
	}

	RobotBase::~RobotBase() {
		m_robot_base_p = nullptr;
	}

	void RobotBase::robotSetup(RobotBase *robot_p) {
		try {
			std::cout << "\r\n********** Robot program starting **********\r\n" << std::endl;
			robot_p->StartCompetition();
		}
		catch (const std::exception &ex)
		{
			std::cout << "\r\n******* Robot threw exception - " << ex.what() << " **************\r\n" << std::endl;
		}
	}

	bool RobotBase::IsDisabled() const {
		return !m_enabled;
	}

	bool RobotBase::IsAutonomous() const {
		return m_mode == RobotMode::Autonomous;
	}

	bool RobotBase::IsEnabled() const {
		return m_enabled;
	}

	bool RobotBase::IsTest() const {
		return m_mode == RobotMode::Test;
	}

	bool RobotBase::IsOperatorControl() const {
		return m_mode == RobotMode::Operator;
	}
}
