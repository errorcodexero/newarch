#include <iostream>
#include <string>
#include <cmath>

constexpr const double PI = 3.14159265359;
constexpr const double Gravity = -9.8;          // m/s
double dragCoefficient = 0.416;                 // Unitless
double powerCellMass = 0.141748;                // kg
double powerCellDiameter = 0.1778;              // m
double airDensity = 1.2;                        // kg/m^3
double shooterHeight = 0.9144;                  // m
double targetHeight = 2.49555;                  // m (98.25 in)
double startAngle;
double endAngle;
double startVelocity;
double endVelocity;

inline double metersToInches(double m)
{
	return m / 0.0254;
}

inline double inchesToMeters(double i)
{
	return i * 0.0254;
}

bool parseDouble(const std::string& str, double& v)
{
	bool ret = true;
	size_t index;

	try {
		v = std::stod(str, &index);
		if (index != str.length())
			ret = false;
	}
	catch (...)
	{
		ret = false;
	}

	return ret;
}

//
// angle in degrees
// velocity in meters per second
//
bool distAtTargetHeight(double angle, double velocity, double& dist)
{
	bool first = true;
	double t = 0.0;
	double crossSection = (powerCellDiameter / 2.0) * (powerCellDiameter / 2.0) * PI;
	double k = airDensity * crossSection * dragCoefficient;
	double y = shooterHeight;
	double x = 0.0;
	double vx = velocity * std::cos(angle / 180.0 * PI);
	double vy = velocity * std::sin(angle / 180.0 * PI);
	double xnext, ynext;
	double deltat = 0.001;
	double calcdt = 0.0;

	while (true)
	{
		double forcex = -k * 0.5 * vx * vx;
		double accelx = forcex / powerCellMass;

		ynext = shooterHeight + vy * t + 0.5 * Gravity * t * t;
		xnext = x + vx * calcdt + 0.5 * accelx * calcdt * calcdt;

		calcdt = deltat;
		vx += deltat * accelx;

		if (y > targetHeight&& ynext < targetHeight)
		{
			break;
		}
		else if (ynext < y && ynext < targetHeight)
		{
			//
			// Never reached the target height
			//
			return false;
		}

		y = ynext;
		x = xnext;

		t += deltat;

	}

	//
	// Ok, we reached the height, the distance (x) is the
	//
	double pcnt = (y - targetHeight) / (y - ynext);
	dist = x + pcnt * (xnext - x);

	return true;
}

int main(int ac, char** av)
{
	ac--;
	av++;

	while (ac-- > 0)
	{
		std::string arg = *av++;

		if (arg == "--mass")
		{
			if (ac == 0)
			{
				std::cerr << "--mass option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, powerCellMass))
			{
				std::cerr << "argument following --mass option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--drag")
		{
			if (ac == 0)
			{
				std::cerr << "--drag option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, dragCoefficient))
			{
				std::cerr << "argument following --drag option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--diameter")
		{
			if (ac == 0)
			{
				std::cerr << "--diameter option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, powerCellDiameter))
			{
				std::cerr << "argument following --diameter option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--density")
		{
			if (ac == 0)
			{
				std::cerr << "--density option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, airDensity))
			{
				std::cerr << "argument following --density option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else
		{
			std::cerr << "invalid option '" << arg << "'" << std::endl;
			return 1;
		}
	}

	for (double angle = 44; angle <= 46; angle++)
	{
		for (double velocity = 20; velocity < 60; velocity += 1)
		{
			double dist;

			double vm = inchesToMeters(velocity * 12.0) ;

			if (distAtTargetHeight(angle, vm, dist))
			{
				dist = metersToInches(dist) / 12.0;

				std::cout << angle << ", " << velocity << ", " << dist << std::endl;
			}
			else
			{
				std::cout << angle << ", " << velocity << ", -" << std::endl;
			}
		}
	}
} 