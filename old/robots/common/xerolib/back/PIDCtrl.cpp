#include "PIDCtrl.h"
#include <iostream>

namespace xerolib
{
	PIDCtrl::PIDCtrl():PIDCtrl(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
	{
	}

	PIDCtrl::PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool isAngle)
	{
		Init(p, i, d, f, floor, ceil, integralCeil);
		current = 0;
		m_count = 0;
	}

	void PIDCtrl::Init(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool isangle)
	{
		PIDConsts.p = p;
		PIDConsts.i = i;
		PIDConsts.d = d;
		PIDConsts.f = f;
		PIDConsts.floor = floor;
		PIDConsts.ceil = ceil;
		PIDConsts.integralCeil = integralCeil;
		mIsAngle = isangle ;
		integral = 0;
	}

	double PIDCtrl::getOutput(double target, double current, double timeDifference, double *pv, double *iv, double *dv, double *fv)
	{
		double error = calcError(target, current) ;
		double pOut = PIDConsts.p*error;
		double derivative = (current - this->current) / timeDifference;
		double dOut = PIDConsts.d*derivative;

		integral += error*timeDifference;
	
		if (integral > PIDConsts.integralCeil)
			integral = PIDConsts.integralCeil;
		else if (integral < -PIDConsts.integralCeil)
			integral = -PIDConsts.integralCeil;
	
		double iOut = PIDConsts.i * integral;

		if (pv != nullptr)
			*pv = pOut ;

		if (iv != nullptr)
			*iv = iOut ;

		if (dv != nullptr)
			*dv = dOut ;

		if (fv != nullptr)
			*fv = PIDConsts.f ;
	
		double output = pOut + iOut + dOut + PIDConsts.f * target;
	
		if (output <= PIDConsts.floor)
			output = PIDConsts.floor;
	
		if (output >= PIDConsts.ceil)
			output = PIDConsts.ceil;

		if (m_logfile.is_open())
		{
			m_logfile << m_count;
			m_logfile << "," << target;
			m_logfile << "," << current;
			m_logfile << "," << output;
			m_logfile << "," << timeDifference;
			m_logfile << "," << integral;
			m_logfile << "," << pOut;
			m_logfile << "," << iOut;
			m_logfile << "," << dOut;
			m_logfile << "," << error;
			m_logfile << "," << derivative;
			m_logfile << std::endl;
		}

		m_count++;
		return output;
	}

	double PIDCtrl::calcError(double target, double current)
	{
		double error = target - current ;
		if (mIsAngle)
		{
			while (error > 180)
				error -= 360 ;

			while (error < -180)
				error += 360 ;
		}

		return error ;
	}
}
