#include "pidctrl.h"
#include <iostream>
#include <limits>

namespace xero {
	namespace misc {
				
		PIDCtrl::PIDCtrl():PIDCtrl(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
		{
		}

		PIDCtrl::PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool isAngle)
		{
			Init(p, i, d, f, floor, ceil, integralCeil);
			current = 0;
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

		void PIDCtrl::Init(double p, double i, double d, double f, bool isangle)
		{
			PIDConsts.p = p;
			PIDConsts.i = i;
			PIDConsts.d = d;
			PIDConsts.f = f;
			PIDConsts.floor = std::numeric_limits<double>::min() ;
			PIDConsts.ceil = std::numeric_limits<double>::max() ;
			PIDConsts.integralCeil = std::numeric_limits<double>::max() ;
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
			
			double output = pOut + iOut + dOut + PIDConsts.f;

		#ifdef PRINT_PID_INTERNALS
			std::cout << "integral " << integral ;
			std::cout << ", pOut " << pOut ;
			std::cout << ", iOut " << iOut ;
			std::cout << ", dOut " << dOut ;
			std::cout << ", error " << error;
			std::cout << ", derivative " << derivative;
			std::cout << std::endl ;
		#endif
			
			if (output <= PIDConsts.floor)
				output = PIDConsts.floor;
			
			if (output >= PIDConsts.ceil)
				output = PIDConsts.ceil;
			
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
}
