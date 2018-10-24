#pragma once

namespace frc
{
	class SmartDashboard
	{
	public:
		SmartDashboard()
		{
		}

		virtual ~SmartDashboard()
		{
		}

		static void PutNumber(const char *label_p, double number)
		{
		}

		static void PutNumber(const std::string &name, double number)
		{
		}

		static void PutString(const char *label_p, const char *string_p)
		{
		}
	};
}

