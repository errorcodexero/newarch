#pragma once

#include "SettingsParser.h"

namespace xero {
namespace misc {

/// \brief A PID controller
class PIDCtrl
{
public:
	/// \brief create a new pid controller with all zero constants
	PIDCtrl();

	/// \brief create a new pid controller with the given constants
	/// \param p the P constant (proportional)
	/// \param i the I constant (integral)
	/// \param d the D constant (difference)
	/// \param f the F constant (feed forward)
	/// \param floor the lowest possible value for the output
	/// \param ceil the highest possible value for the output
	/// \param integralCeil the largest magnitude for the stored integral sum
	/// \param is_angle if true the values are angles that wrap at +/- 180
	PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool is_angle = false);

	/// \brief Initialize the PID controller with a new set of constants
	/// \param p the P constant (proportional)
	/// \param i the I constant (integral)
	/// \param d the D constant (difference)
	/// \param f the F constant (feed forward)
	/// \param floor the lowest possible value for the output
	/// \param ceil the highest possible value for the output
	/// \param integralCeil the largest magnitude for the stored integral sum
	/// \param is_angle if true the values are angles that wrap at +/- 180
	void init(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool is_angle = false);

	/// \brief Initialize the PID controller with a new set of constants
	/// \param p the P constant (proportional)
	/// \param i the I constant (integral)
	/// \param d the D constant (difference)
	/// \param f the F constant (feed forward)
	/// \param is_angle if true the values are angles that wrap at +/- 180
	void init(double p, double i, double d, double f, bool is_angle = false);

	/// \brief Initialize the PID controller from constants in the settings
	///
	/// Used settings: double :p, double :i, double :d, double :f
	/// \param parser the settings parser
	/// \param prefix the prefix under which to find the PID constants in the settings file
	/// \param is_angle if true the values are angles that wrap at +/- 180
	void initFromSettings(SettingsParser &parser, const std::string &prefix, bool is_angle = false);

	/// \brief Initialize the PID controller from constants in the settings with additional PID options (namely the min, max, and imax)
	///
	/// Used settings: double :p, double :i, double :d, double :f, double :min, double :max, double :imax
	/// \param parser the settings parser
	/// \param prefix the prefix under which to find the PID constants in the settings file
	/// \param is_angle if true the values are angles that wrap at +/- 180
	void initFromSettingsExtended(SettingsParser &parser, const std::string &prefix, bool is_angle = false);

	/// \brief Return the output given a target, the current value, and the time that has passed
	/// \param target the target value we are trying to reach
	/// \param current the current value for system
	/// \param timeDifference the time that has passed since the last time this was called
	/// \param pv the calculated p component
	/// \param iv the calculated i component
	/// \param dv the calculated d component
	/// \param fv the calculated f component
	/// \returns the calculated output value
	double getOutput(double target, double current, double timeDifference,
					double *pv = nullptr, double *iv = nullptr, double *dv = nullptr, double *fv = nullptr);

	std::string toString();
	
	/// \brief return the internally stored sum
	double getInternalSum() const
	{
		return integral_;
	}

private:
	double calcError(double target, double current);

private:
	struct PIDConstants
	{
		double p, i, d, f;
		double floor, ceil;
		double integralCeil;
	} pid_consts_;

	bool is_angle_;
	double target_;
	double current_;
	double integral_;
};

} // namespace base
} // namespace xero
