// DevApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "WaypointReader.h"
#include "CSVWriter.h"
#include "PathGenerator.h"
#include "DistanceView.h"
#include "TimingUtil.h"
#include <iostream>

using namespace xero::paths;

int main(int ac, char **av)
{
	ac--;
	av++;

	double maxDx = 2.0;
	double maxDy = 0.25;
	double maxDTheta = MathUtils::degreesToRadians(5.0);
	double startvel = 0.0;
	double maxvel = 130.0;
	double endvel = 0.0;
	double maxaccel = 130.0;

	//
	// Process the command line arguments
	//
	while (ac > 0 && **av == '-')
	{
		std::string arg = *av;
		if (arg == "--dx")
		{

		}
		else if (arg == "--dy")
		{

		}
		else if (arg == "--dtheta")
		{

		}
		else 
		{
			std::cerr << "devapp: invalid command line argument '" << arg << "'" << std::endl;
			return 1;
		}
	}

	if (ac != 3)
	{
		std::cerr << "devapp: usage: devapp waypoint_file splinefile pathfile" << std::endl;
		return 1;
	}

	std::string wayfile = *av++;
	std::string splinefile = *av++;
	std::string pathfile = *av++;

	std::vector<Pose2d> waypoints;
	if (!WaypointReader::readWaypoints(wayfile, waypoints))
	{
		std::cerr << "devapp: error reading waypoints from waypoint file" << std::endl;
		return 1;
	}

	PathGenerator gen(maxDx, maxDy, maxDTheta);
	Trajectory trajectory = gen.generate(waypoints);
	CSVWriter::write<std::vector<TrajectoryPoint>::const_iterator>(splinefile, trajectory.getPoints().begin(), trajectory.getPoints().end());

	ConstraintList clist;

	DistanceView dist(trajectory);
	TimedTrajectory ttj = TimingUtil::timeParameterizeTrajectory(false, dist, clist, 2.0, startvel, endvel, maxvel, maxaccel);
	CSVWriter::write< std::vector<TimedTrajectoryPoint>::const_iterator>(pathfile, ttj.getPoints().begin(), ttj.getPoints().end());

	return 0;
}
