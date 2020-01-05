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

double startAngle = 45.0;						// degrees
double endAngle = 45.0;							// degrees
double angleStep = 1.0;							// degrees
double startVelocity = 1.0;						// ft / sec
double endVelocity = 45.0;						// ft / sec
double velocityStep = 1.0;						// ft / sec

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

void usage()
{

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
		else if (arg == "--startAngle")
		{
			if (ac == 0)
			{
				std::cerr << "--startAngle option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, startAngle))
			{
				std::cerr << "argument following --startAngle option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--endAngle")
		{
			if (ac == 0)
			{
				std::cerr << "--endAngle option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, endAngle))
			{
				std::cerr << "argument following --endAngle option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--angleStep")
		{
			if (ac == 0)
			{
				std::cerr << "--angleStep option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, angleStep))
			{
				std::cerr << "argument following --angleStep option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--startVelocity")
		{
			if (ac == 0)
			{
				std::cerr << "--startVelocity option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, startVelocity))
			{
				std::cerr << "argument following --startVelocity option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--endVelocity")
		{
			if (ac == 0)
			{
				std::cerr << "--endVelocity option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, endVelocity))
			{
				std::cerr << "argument following --endVelocity option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else if (arg == "--velocityStep")
		{
			if (ac == 0)
			{
				std::cerr << "--velocityStep option requires one additional argument" << std::endl;
				return 1;
			}

			arg = *av++;
			if (!parseDouble(arg, velocityStep))
			{
				std::cerr << "argument following --velocityStep option must be a floating point number" << std::endl;
				return 1;
			}
		}
		else
		{
			usage();
			std::cerr << "invalid option '" << arg << "'" << std::endl;
			return 1;
		}
	}

	std::cout << "angle, velocity, distance" << std::endl;

	double angle = startAngle;
	do {
		double velocity = startVelocity;
		do
		{
			double dist;

			// Convert velocity from ft/sec to m/sec
			double vm = inchesToMeters(velocity * 12.0);

			// Get distance traveled from shooter height to target height
			if (distAtTargetHeight(angle, vm, dist))
			{
				// Convert to feet
				dist = metersToInches(dist) / 12.0;
				std::cout << angle << ", " << velocity << ", " << dist << std::endl;
			}
			velocity += velocityStep;
		} while (velocity < endVelocity);

		angle += angleStep;

	} while (angle < endAngle);
}
