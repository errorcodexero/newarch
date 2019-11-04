#include "DriveBase.h"
#include "XeroRobotBase.h"
#include "RobotState.h"
#include "ParamsParser.h"
#include "RobotStateEstimator.h"
#include "Kinematics.h"
#include "ModuleDefintions.h"
#include "PathDebugData.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>

#define LOGPATH

using namespace xero::math;
using namespace xero::motion;
using namespace xero::pathfinder;

namespace xerolib
{
	DriveBase::DriveBase(XeroRobotBase &robot,
						 std::shared_ptr<frc::SpeedController> left_motor_p, std::shared_ptr<frc::SpeedController> right_motor_p,
						 std::shared_ptr<frc::Encoder> left_enc_p,  std::shared_ptr<frc::Encoder> right_enc_p,
						 std::shared_ptr<AHRS> navx_p) : DriveBase(robot)
	{
		setMotors(left_motor_p, right_motor_p) ;
		setEncoders(left_enc_p, right_enc_p) ;
		setNavX(navx_p) ;
	}
	
	DriveBase::DriveBase(XeroRobotBase &robot,
						 std::list<std::shared_ptr<frc::SpeedController>> left_motors, std::list<std::shared_ptr<frc::SpeedController>> right_motors,
						 std::shared_ptr<frc::Encoder> left_enc_p,  std::shared_ptr<frc::Encoder> right_enc_p,
						 std::shared_ptr<AHRS> navx_p) : DriveBase(robot)
	{
		setMotors(left_motors, right_motors) ;
		setEncoders(left_enc_p, right_enc_p) ;
		setNavX(navx_p) ;
	}

	DriveBase::DriveBase(XeroRobotBase &robot) : SubsystemBase("drivebase", robot)
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

	DriveBase::~DriveBase()
	{
		m_running = false;
		m_velocity_thread.join();
		m_drivebase_control_thread.join();
		m_estimator_thread.join();
	}

	void DriveBase::startThreads()
	{
		if (m_left_motors.size() > 0 && m_right_motors.size() > 0 && m_left_encoder_p != nullptr && m_right_encoder_p != nullptr && m_navx_p != nullptr)
		{
			m_running = true;
			m_velocity_thread = std::thread(&DriveBase::velocityThread, this);
			m_drivebase_control_thread = std::thread(&DriveBase::controlThread, this);
			m_estimator_thread = std::thread(&DriveBase::stateThread, this);
		}
	}

	void DriveBase::stateThread()
	{
		std::chrono::milliseconds delay(5);
		RobotStateEstimator &est = RobotStateEstimator::get();
		est.setDriveBase(this);

		while (m_running)
		{
			double time = frc::Timer::GetFPGATimestamp();
			est.oneLoop(time);
			std::this_thread::sleep_for(delay);
		}
	}

	void DriveBase::controlThread()
	{
		std::chrono::milliseconds delay(25);

		while (m_running)
		{
			std::this_thread::sleep_for(delay);
		}
	}

	void DriveBase::velocityThread()
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

			if (m_mode == Mode::Path || m_mode == Mode::Velocity)
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

	void DriveBase::initSpeedometers()
	{
		ParamsParser &params = ParamsParser::get();

		double samples = params.getValue("drivebase:speedometer:samples");
		size_t count = static_cast<size_t>(samples + 0.5);
		m_left_speed.setSampleCount(count);
		m_right_speed.setSampleCount(count);
	}

	void DriveBase::initPIDConstants()
	{
		ParamsParser &params = ParamsParser::get();

		double kp = params.getValue("drivebase:velocity:p");
		double ki = params.getValue("drivebase:velocity:i");
		double kd = params.getValue("drivebase:velocity:d");
		double kf = params.getValue("drivebase:velocity:f");
		double vmin = params.getValue("drivebase:velocity:vmin") ;
		double vmax = params.getValue("drivebase:velocity:vmax") ;

		m_left_velocity_pid.Init(kp, ki, kd, kf, vmin, vmax, 1e6) ;
		m_right_velocity_pid.Init(kp, ki, kd, kf, vmin, vmax, 1e6);
	}

	void DriveBase::setVelocities(double left, double right)
	{
		std::lock_guard<std::mutex> lock(m_velocity_mutex);
		m_left_target_velocity = left;
		m_right_target_velocity = right;
	}

	double DriveBase::getDistance()
	{
		int32_t left = m_left_encoder_p->Get();
		int32_t right = m_right_encoder_p->Get();
		int32_t avg = (left + right) / 2;
		double rev = (double)avg / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	double DriveBase::getLeftDistance()
	{
		int32_t left = m_left_encoder_p->Get();
		double rev = (double)left / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	double DriveBase::getRightDistance()
	{
		int32_t right = m_right_encoder_p->Get();
		double rev = (double)right / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	void DriveBase::getInputs()
	{
		m_yaw = -m_navx_p->GetYaw();
	}

	void DriveBase::setOutputs()
	{
		if (m_mode == Mode::Manual)
			outputToMotors();
	}

	void DriveBase::outputToMotors()
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
