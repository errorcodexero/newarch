#include "TankDrive.h"
#include "XeroRobotBase.h"
#include "ParamsParser.h"
#include "ModuleDefintions.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>

#define LOGPATH

namespace xero
{
	namespace base
	{
		TankDrive::TankDrive(XeroRobotBase &robot,
			std::shared_ptr<frc::SpeedController> left_motor_p, std::shared_ptr<frc::SpeedController> right_motor_p,
			std::shared_ptr<frc::Encoder> left_enc_p, std::shared_ptr<frc::Encoder> right_enc_p,
			std::shared_ptr<AHRS> navx_p) : TankDrive(robot)
		{
			setMotors(left_motor_p, right_motor_p);
			setEncoders(left_enc_p, right_enc_p);
			setNavX(navx_p);
		}

		TankDrive::TankDrive(XeroRobotBase &robot,
			std::list<std::shared_ptr<frc::SpeedController>> left_motors, std::list<std::shared_ptr<frc::SpeedController>> right_motors,
			std::shared_ptr<frc::Encoder> left_enc_p, std::shared_ptr<frc::Encoder> right_enc_p,
			std::shared_ptr<AHRS> navx_p) : TankDrive(robot)
		{
			setMotors(left_motors, right_motors);
			setEncoders(left_enc_p, right_enc_p);
			setNavX(navx_p);
		}

		TankDrive::TankDrive(XeroRobotBase &robot) : DriveBase("TankDrive", robot)
		{
			m_mode = Mode::Idle;

			m_left_voltage = 0.0;
			m_right_voltage = 0.0;
			m_left_last_voltage = std::nan("");
			m_right_last_voltage = std::nan("");


			initSpeedometers();
			initPIDConstants();
			m_yaw = 0.0;
		}

		TankDrive::~TankDrive()
		{
			m_running = false;
			m_velocity_thread.join();
			m_TankDrive_control_thread.join();
			m_estimator_thread.join();
		}

		void TankDrive::setPhysicalChar(int32_t ticks, double diam, double width, double scrub)
		{
			m_ticks_per_rev = ticks;
			m_wheel_diameter = diam;
			m_width = width;
			m_scrub = scrub;
		}

		void TankDrive::startThreads()
		{
			if (m_left_motors.size() > 0 && m_right_motors.size() > 0 && m_left_encoder_p != nullptr && m_right_encoder_p != nullptr && m_navx_p != nullptr)
			{
				m_running = true;
				m_velocity_thread = std::thread(&TankDrive::velocityThread, this);
				m_TankDrive_control_thread = std::thread(&TankDrive::controlThread, this);
				m_estimator_thread = std::thread(&TankDrive::stateThread, this);
			}
		}

		void TankDrive::resetPosition(const PositionAngle &pa)
		{
		}

		void TankDrive::stateThread()
		{
			std::chrono::milliseconds delay(5);

			while (m_running)
			{
				double time = frc::Timer::GetFPGATimestamp();
				std::this_thread::sleep_for(delay);
			}
		}

		void TankDrive::controlThread()
		{
			std::chrono::milliseconds delay(25);

			while (m_running)
			{
				std::this_thread::sleep_for(delay);
			}
		}

		void TankDrive::velocityThread()
		{
			std::chrono::milliseconds delay(25);
			bool first = true;
			double current = frc::Timer::GetFPGATimestamp();
			double last = current;
			bool updated;

			m_left_velocity_pid.setLogFile(getRobot().getBaseDir() + "\\leftpid.csv");
			m_right_velocity_pid.setLogFile(getRobot().getBaseDir() + "\\rightpid.csv");

			while (m_running)
			{
				updated = false;
				current = frc::Timer::GetFPGATimestamp();

				if (m_mode == Mode::Velocity)
				{
					if (!first)
					{
						double lefttarget, righttarget;
						double leftactual, rightactual;
						{
							//
							// Synchronize access to the left and right target velocties
							//
							std::lock_guard<std::mutex> lock(m_velocity_mutex);
							double ldist = getLeftDistance();
							double rdist = getRightDistance();
							m_left_speed.addSample(current, ldist);
							m_right_speed.addSample(current, rdist);
							lefttarget = m_left_target_velocity;
							righttarget = m_right_target_velocity;
							leftactual = m_left_speed.getSpeed();
							rightactual = m_right_speed.getSpeed();
							updated = true;
						}

						double dt = current - last;
						m_left_voltage = m_left_velocity_pid.getOutput(lefttarget, leftactual, dt);
						m_right_voltage = m_right_velocity_pid.getOutput(righttarget, rightactual, dt);

						outputToMotors();
					}
				}

				if (!updated)
				{
					std::lock_guard<std::mutex> lock(m_velocity_mutex);
					m_left_speed.addSample(current, getLeftDistance());
					m_right_speed.addSample(current, getRightDistance());
				}

				first = false;
				last = current;
				std::this_thread::sleep_for(delay);
			}
		}

		void TankDrive::initSpeedometers()
		{
			ParamsParser &params = ParamsParser::get();

			double samples = params.getValue("TankDrive:speedometer:samples");
			size_t count = static_cast<size_t>(samples + 0.5);
			m_left_speed.setSampleCount(count);
			m_right_speed.setSampleCount(count);
		}

		void TankDrive::initPIDConstants()
		{
			ParamsParser &params = ParamsParser::get();

			double kp = params.getValue("TankDrive:velocity:p");
			double ki = params.getValue("TankDrive:velocity:i");
			double kd = params.getValue("TankDrive:velocity:d");
			double kf = params.getValue("TankDrive:velocity:f");
			double vmin = params.getValue("TankDrive:velocity:vmin");
			double vmax = params.getValue("TankDrive:velocity:vmax");

			m_left_velocity_pid.Init(kp, ki, kd, kf, vmin, vmax, 1e6);
			m_right_velocity_pid.Init(kp, ki, kd, kf, vmin, vmax, 1e6);

		}

		void TankDrive::setVelocities(double left, double right)
		{
			std::lock_guard<std::mutex> lock(m_velocity_mutex);
			m_left_target_velocity = left;
			m_right_target_velocity = right;
		}

		double TankDrive::getDistance()
		{
			int32_t left = m_left_encoder_p->Get();
			int32_t right = m_right_encoder_p->Get();
			int32_t avg = (left + right) / 2;
			double rev = (double)avg / (double)m_ticks_per_rev;
			return rev * m_wheel_diameter * PI;
		}

		double TankDrive::getLeftDistance()
		{
			int32_t left = m_left_encoder_p->Get();
			double rev = (double)left / (double)m_ticks_per_rev;
			return rev * m_wheel_diameter * PI;
		}

		double TankDrive::getRightDistance()
		{
			int32_t right = m_right_encoder_p->Get();
			double rev = (double)right / (double)m_ticks_per_rev;
			return rev * m_wheel_diameter * PI;
		}

		void TankDrive::getInputs()
		{
			m_yaw = -m_navx_p->GetYaw();
		}

		void TankDrive::setOutputs()
		{
			if (m_mode == Mode::Manual)
			{
				outputToMotors();
			}
		}

		void TankDrive::outputToMotors()
		{
			std::lock_guard<std::mutex> lock(m_motor_voltage_lock);

			if (std::isnan(m_left_last_voltage) || std::fabs(m_left_last_voltage - m_left_voltage) < kDelta)
			{
				for (auto motor_p : m_left_motors)
					motor_p->Set(m_left_voltage);
			}

			if (std::isnan(m_right_last_voltage) || std::fabs(m_right_last_voltage - m_right_voltage) < kDelta)
			{
				for (auto motor_p : m_right_motors)
					motor_p->Set(m_right_voltage);
			}
		}
	}
}