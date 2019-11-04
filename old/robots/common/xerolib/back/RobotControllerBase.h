#pragma once

namespace xerolib
{
    class XeroRobotBase;

    class RobotControllerBase
    {
    public:
	RobotControllerBase(XeroRobotBase &robot);
	virtual ~RobotControllerBase();

	virtual bool isRunning()
	{
		return true;
	}

	virtual void executeOneLoop() = 0;

	XeroRobotBase &getRobot()
	{
	    return m_robot;
	}

    private:
	XeroRobotBase &m_robot;
    };
}
