#pragma once

class PIDCtrl {
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
	PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool isangle = false);

	
	/// \brief Initialize the PID controller with a new set of constants
	/// \param p the P constant (proportional)
	/// \param i the I constant (integral)
	/// \param d the D constant (difference)
	/// \param f the F constant (feed forward)
	/// \param floor the lowest possible value for the output
	/// \param ceil the highest possible value for the output
	/// \param integralCeil the largest magnitude for the stored integral sum
	/// \param isangle if true the values are angles that wrap at +/- 180
	void Init(double p, double i, double d, double f, double floor, double ceil, double integralCeil, bool isangle = false);

	/// \brief Return the output given a target, the current value, and the time that has passed
	/// \param target the target value we are trying to reach
	/// \param current the current value for system
	/// \param the time that has passed since the last time this was called
	double getOutput(double target, double current, double timeDifference,
					 double *pv = nullptr, double *iv=nullptr, double *dv=nullptr, double *fv=nullptr);

	/// \brief return the internally stored sum
	double getInternalSum() const
	{
		return integral ;
	}

private:
	double calcError(double target, double current) ;

private:
	struct PIDConstants
	{
		double p, i, d, f;
		double floor, ceil;
		double integralCeil;
	} PIDConsts;

	bool mIsAngle ;
	double target ;
	double current ;
	double timeDifference;
	double integral;
};
