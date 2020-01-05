#include "ShooterSolver.h"
#include <cmath>
ShooterSolver::ShooterSolver(double cross, double drag, double mass)
{
	crossSection_ = cross;					// ft^2
	dragCoefficient_ = drag;				// unitless
	mass_ = mass;							// lbs
	gravity_ = 32.0;						// ft/sec
	airDensity_ = 0.0765;					// lbs/ft^3
}

bool ShooterSolver::distanceAtTargetHeight(double angle, double velocity, double shooter, double target, double& dist)
{
	double t = 0.0;
	double k = airDensity_ * crossSection_ * dragCoefficient_;
	double y = shooter;
	double x = 0.0;
	double vx = velocity * std::cos(angle / 180.0 * PI);
	double vy = velocity * std::sin(angle / 180.0 * PI);
	double xnext, ynext;
	double deltat = 0.001;
	double calcdt = 0.0;


}

bool ShooterSolver::heightAtTargetDistance(double angle, double velocity, double shooter, double target, double& height)
{

}