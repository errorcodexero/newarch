#pragma once

#include<string>

namespace xerolib
{
	class XeroRobotBase;

	class SubsystemBase
	{
	public:
		SubsystemBase(const char *name_p, XeroRobotBase &robot);
		~SubsystemBase();

		const std::string &getName() const
		{
			return m_name;
		}

		virtual void getInputs() = 0;
		virtual void setOutputs() = 0;

	protected:
		XeroRobotBase &getRobot() 
		{
			return m_robot;
		}

	private:
		XeroRobotBase &m_robot;
		std::string m_name;
	};
}
