#include "PIDCtrl.h"

#include <iostream>
#include <limits>
#include <sstream>

#include "xeromath.h"

namespace xero {
namespace misc {

PIDCtrl::PIDCtrl():PIDCtrl(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
{
}

PIDCtrl::PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool is_angle)
{
	init(p, i, d, f, floor, ceil, integralCeil, is_angle);
	current_ = 0;
}

void PIDCtrl::init(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool is_angle)
{
	pid_consts_.p = p;
	pid_consts_.i = i;
	pid_consts_.d = d;
	pid_consts_.f = f;
	pid_consts_.floor = floor;
	pid_consts_.ceil = ceil;
	pid_consts_.integralCeil = integralCeil;
	is_angle_ = is_angle ;
	integral_ = 0;
}

void PIDCtrl::init(double p, double i, double d, double f, bool is_angle)
{
	pid_consts_.p = p;
	pid_consts_.i = i;
	pid_consts_.d = d;
	pid_consts_.f = f;
	pid_consts_.floor = std::numeric_limits<double>::min() ;
	pid_consts_.ceil = std::numeric_limits<double>::max() ;
	pid_consts_.integralCeil = std::numeric_limits<double>::max() ;
	is_angle_ = is_angle ;
	integral_ = 0;
}

void PIDCtrl::initFromSettings(SettingsParser &parser, const std::string &prefix, bool is_angle) {
	pid_consts_.p = parser.getDouble(prefix + ":p");
	pid_consts_.i = parser.getDouble(prefix + ":i");
	pid_consts_.d = parser.getDouble(prefix + ":d");
	pid_consts_.f = parser.getDouble(prefix + ":f");
	pid_consts_.floor = std::numeric_limits<double>::min() ;
	pid_consts_.ceil = std::numeric_limits<double>::max() ;
	pid_consts_.integralCeil = std::numeric_limits<double>::max() ;
	is_angle_ = is_angle ;
	integral_ = 0;
}

void PIDCtrl::initFromSettingsExtended(SettingsParser &parser, const std::string &prefix, bool is_angle) {
	initFromSettings(parser, prefix, is_angle);

	pid_consts_.floor = parser.getDouble(prefix + ":min");
	pid_consts_.ceil = parser.getDouble(prefix + ":max");
	pid_consts_.integralCeil = parser.getDouble(prefix + ":imax");
}

double PIDCtrl::getOutput(double target, double current, double timeDifference, double *pv, double *iv, double *dv, double *fv)
{
	double error = calcError(target, current) ;
	double pOut = pid_consts_.p*error;
	double derivative = (current - current_) / timeDifference;
	double dOut = pid_consts_.d*derivative;
	
	integral_ += error*timeDifference;
	
	if (integral_ > pid_consts_.integralCeil)
		integral_ = pid_consts_.integralCeil;
	else if (integral_ < -pid_consts_.integralCeil)
		integral_ = -pid_consts_.integralCeil;
	
	double iOut = pid_consts_.i * integral_;

	if (pv != nullptr)
		*pv = pOut ;

	if (iv != nullptr)
		*iv = iOut ;

	if (dv != nullptr)
		*dv = dOut ;

	if (fv != nullptr)
		*fv = pid_consts_.f ;
	
	double output = pOut + iOut + dOut + pid_consts_.f * target ;

	if (output <= pid_consts_.floor)
		output = pid_consts_.floor;
	
	if (output >= pid_consts_.ceil)
		output = pid_consts_.ceil;
	
	return output;
}

double PIDCtrl::calcError(double target, double current)
{
	double error ;
	
	if (is_angle_)
		error = xero::math::normalizeAngleDegrees(target - current) ;
	else
		error = target - current ;

	return error ;
}

std::string PIDCtrl::toString() {
	std::stringstream ss;
	ss << "p: " << pid_consts_.p;
	ss << ",i: " << pid_consts_.i;
	ss << ",d: " << pid_consts_.d;
	ss << ",f: " << pid_consts_.f;
	ss << ",min: " << pid_consts_.floor;
	ss << ",max: " << pid_consts_.ceil;
	ss << ",imax: " << pid_consts_.integralCeil;
	ss << ",is_angle: " << is_angle_;
	ss << ",target: " << target_;
	ss << ",current: " << current_;
	ss << ",integral: " << integral_;
	return ss.str();
}

} // namespace base
} // namespace xero
