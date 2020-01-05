#pragma once
class ShooterSolver
{
	constexpr static const double PI = 3.14159265359;

public:
	ShooterSolver(double cross, double drag, double mass);

	bool distanceAtTargetHeight(double angle, double velocity, double shooter, double target, double& dist);
	bool heightAtTargetDistance(double angle, double velocity, double shooter, double target, double& height);

private:
	double crossSection_;
	double dragCoefficient_;
	double mass_;
	double airDensity_;
	double gravity_;
};

