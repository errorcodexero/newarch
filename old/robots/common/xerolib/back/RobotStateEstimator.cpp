#include "RobotStateEstimator.h"
#include "PositionAngle.h"
#include "RobotState.h"
#include "Kinematics.h"
#include <iostream>

using namespace xero::math;
using namespace xero::motion;

namespace xero
{
	namespace pathfinder
	{
		RobotStateEstimator *RobotStateEstimator::TheOneObject = nullptr;
		std::string RobotStateEstimator::LogFileName;

		RobotStateEstimator::RobotStateEstimator(const std::string &name)
		{
			m_left_dist_prev = 0.0;
			m_right_dist_prev = 0.0;

			m_logfile.open(name);
			if (m_logfile.is_open())
			{
				m_logfile << "t";
				m_logfile << ",distx";
				m_logfile << ",disty";
				m_logfile << ",angle";
				m_logfile << ",speedx";
				m_logfile << ",speedy";
				m_logfile << ",odx";
				m_logfile << ",ody";
				m_logfile << ",oda";
				m_logfile << ",prx";
				m_logfile << ",pry";
				m_logfile << ",pra";
				m_logfile << ",patx";
				m_logfile << ",paty";
				m_logfile << ",pata";
				m_logfile << std::endl;
			}
		}

		RobotStateEstimator::~RobotStateEstimator()
		{
		}

		void RobotStateEstimator::oneLoop(double t)
		{
			RobotState &st = RobotState::get();

			double left = m_db_p->getLeftDistance();
			double right = m_db_p->getRightDistance();
			Rotation angle = m_db_p->getAngle();

			double lspeed = m_db_p->getLeftSpeed();
			double rspeed = m_db_p->getRightSpeed();
			PositionAngle od_vel = st.generateOdometryFromSensors(left - m_left_dist_prev, right - m_right_dist_prev, angle);
			PositionAngle predicted_vel = Kinematics::forwardKinematics(lspeed, rspeed, angle.getRadians());
			st.addActualPosition(t, od_vel, predicted_vel);
			m_left_dist_prev = left;
			m_right_dist_prev = right;

			if (m_logfile.is_open())
			{
				auto v = st.getPositionAtTime(t);
				m_logfile << t;
				m_logfile << "," << left;
				m_logfile << "," << right;
				m_logfile << "," << angle.getDegrees();
				m_logfile << "," << lspeed;
				m_logfile << "," << rspeed;
				m_logfile << "," << od_vel.getPosition().getX();
				m_logfile << "," << od_vel.getPosition().getY();
				m_logfile << "," << od_vel.getAngle() / 3.1415926 * 180.0;
				m_logfile << "," << predicted_vel.getPosition().getX();
				m_logfile << "," << predicted_vel.getPosition().getY();
				m_logfile << "," << predicted_vel.getAngle() / 3.1415926 * 180.0;
				m_logfile << "," << v.getPos().getX();
				m_logfile << "," << v.getPos().getY();
				m_logfile << "," << v.getRotation().getDegrees();
				m_logfile << std::endl;
			}
		}
	}
}
