// DevApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "JSONPathReader.h"
#include "PathCollection.h"
#include "CSVWriter.h"
#include "PathGenerator.h"
#include "DistanceView.h"
#include "TimingUtil.h"
#include <iostream>

using namespace xero::paths;

double maxDx = 2.0;
double maxDy = 0.25;
double maxDTheta = MathUtils::degreesToRadians(0.1);
double step = 2.0;
double timestep = 0.02;
std::string pathfile;
std::string outputdir;
std::string outfile;
bool distancefile;
bool splinefile;
xero::paths::PathCollection collection;

extern void generateForGroup(const std::string& group);
extern void generateForPath(PathGroup& group, const std::string& path);

int main(int ac, char** av)
{
	ac--;
	av++;

	//
	// Process the command line arguments
	//
	while (ac > 0 && **av == '-')
	{
		size_t index;

		std::string arg = *av++;
		ac--;

		if (arg == "--dx")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected floating point nubmer following --dx argument" << std::endl;
				return 1;
			}

			arg = *av;
			try {
				maxDx = std::stod(arg, &index);
			}
			catch (...)
			{
				std::cerr << "pathgen: expected floating point nubmer following --dx argument" << std::endl;
				return 1;
			}

			if (index != arg.length()) 
			{
				std::cerr << "pathgen: expected floating point nubmer following --dx argument" << std::endl;
				return 1;
			}
			ac--;
			av++;
		}
		else if (arg == "--dy")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected floating point nubmer following --dy argument" << std::endl;
				return 1;
			}

			arg = *av;
			try {
				maxDy = std::stod(arg, &index);
			}
			catch (...)
			{
				std::cerr << "pathgen: expected floating point nubmer following --dy argument" << std::endl;
				return 1;
			}

			if (index != arg.length())
			{
				std::cerr << "pathgen: expected floating point nubmer following --dy argument" << std::endl;
				return 1;
			}
			ac--;
			av++;
		}
		else if (arg == "--dtheta")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected floating point nubmer following --dtheta argument" << std::endl;
				return 1;
			}

			arg = *av;
			try {
				maxDTheta = std::stod(arg, &index);
				maxDTheta = MathUtils::degreesToRadians(maxDTheta);
			}
			catch (...)
			{
				std::cerr << "pathgen: expected floating point nubmer following --dtheta argument" << std::endl;
				return 1;
			}

			if (index != arg.length())
			{
				std::cerr << "pathgen: expected floating point nubmer following --dtheta argument" << std::endl;
				return 1;
			}
			ac--;
			av++;
		}
		else if (arg == "--step")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected floating point nubmer following --step argument" << std::endl;
				return 1;
			}

			arg = *av;
			try {
				step = std::stod(arg, &index);
			}
			catch (...)
			{
				std::cerr << "pathgen: expected floating point nubmer following --step argument" << std::endl;
				return 1;
			}

			if (index != arg.length())
			{
				std::cerr << "pathgen: expected floating point nubmer following --step argument" << std::endl;
				return 1;
			}
			ac--;
			av++;
		}
		else if (arg == "--timestep")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected floating point nubmer following --timestep argument" << std::endl;
				return 1;
			}

			arg = *av;
			try {
				timestep = std::stod(arg, &index);
			}
			catch (...)
			{
				std::cerr << "pathgen: expected floating point nubmer following --timestep argument" << std::endl;
				return 1;
			}

			if (index != arg.length())
			{
				std::cerr << "pathgen: expected floating point nubmer following --timestep argument" << std::endl;
				return 1;
			}
			ac--;
			av++;
		}
		else if (arg == "--pathfile")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected directory name following --pathfile argument" << std::endl;
				return 1;
			}
			pathfile = *av++;
			ac--;
		}
		else if (arg == "--splinefile")
		{
			splinefile = true;
		}
		else if (arg == "--distancefile")
		{
			distancefile = true;
		}
		else if (arg == "--outdir")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected directory name following --outdir argument" << std::endl;
				return 1;
			}

			if (outfile.length() > 0)
			{
				std::cerr << "pathgen: cannot specify both --outdir and --outfile options" << std::endl;
				return 1;
			}
			outputdir = *av++;
			ac--;
		}
		else if (arg == "--outfile")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected directory name following --output argument" << std::endl;
				return 1;
			}

			if (outputdir.length() > 0)
			{
				std::cerr << "pathgen: cannot specify both --outdir and --outfile options" << std::endl;
				return 1;
			}
			outfile = *av++;
			ac--;
		}
		else
		{
			std::cerr << "pathgen: invalid command line argument '" << arg << "'" << std::endl;
			return 1;
		}
	}

	if (outputdir.length() == 0 && outfile.length() == 0)
		outputdir = ".";

	if (!JSONPathReader::readJSONPathFile(pathfile, collection)) {
		std::cerr << "pathgen: error reading path file '" << pathfile << "'" << std::endl;
		return 1;
	}

	if (outfile.length() > 0 && collection.getPathCount() != 1) {
		std::cerr << "pathgen: --outfile option is only value for a single path" << std::endl;
		return 1;
	}

	std::vector<std::string> groups = collection.getGroupNames();
	for (const std::string& group : groups)
		generateForGroup(group);
}

void generateForGroup(const std::string& group)
{
	auto gptr = collection.findGroupByName(group);
	if (gptr == nullptr) 
	{
		std::cerr << "pathgen: warning: path group '" << group << "' does not exist" << std::endl;
		return;
	}

	std::cout << "Generating paths for group " << group << " ... " << std::endl;
	std::vector<std::string> paths = gptr->getPathNames();
	for(const std::string &path: paths)
		generateForPath(*gptr, path);
}

void generateForPath(PathGroup& group, const std::string& path)
{
	std::cout << "  Generating paths for path " << path << " ... " << std::endl;
	auto pptr = group.findPathByName(path);
	if (pptr == nullptr)
	{
		std::cerr << "pathgen: warning: path '" << path << "' does not exist in group '";
		std::cerr << group.getName() << "'" << std::endl;
		return;
	}

	//
	// Turn the waypoints into a set of irregularly spaced trajectory points.  The spacing will be
	// variable and will be given by the allowed differences between points in the X, Y, and THETA
	// values.
	//
	PathGenerator gen(maxDx, maxDy, maxDTheta);
	Trajectory trajectory = gen.generate(pptr->getPoints());
	if (splinefile && outputdir.length() > 0)
	{
		std::string name = outputdir + "/" + group.getName() + "_" + path + "_spline";
		name += ".csv";
		std::vector<std::string> headers = { "x", "y", "heading", "curvature", "dscurvature" };
		CSVWriter::write<std::vector<TrajectoryPoint>::const_iterator>(name, headers, trajectory.getPoints().begin(), trajectory.getPoints().end());
	}

	//
	// Turn the trajectory points into a set of evenly spaced points along the path.
	//
	DistanceView dist(trajectory);
	if (distancefile && outputdir.length() > 0)
	{
		std::string name = outputdir + "/" + group.getName() + "_" + path + "_distance";
		name += ".csv";

		std::vector<TrajectorySamplePoint> points;
		for (double t = 0.0; t < dist.length(); t += step)
			points.push_back(dist.sample(t));
		std::vector<std::string> headers = { "x", "y", "heading", "curvature", "dscurvature" };
		CSVWriter::write< std::vector<TrajectorySamplePoint>::const_iterator>(name, headers, points.begin(), points.end());
	}

	TimedTrajectory ttj = TimingUtil::timeParameterizeTrajectory(dist, pptr->getConstraints(), step,
		pptr->getStartVelocity(), pptr->getEndVelocity(), pptr->getMaxVelocity(), pptr->getMaxAccel());
	std::string name;
	
	if (outputdir.length())
		name = outputdir + "/" + group.getName() + "_" + path + "_path.csv";
	else
		name = outfile;

	TimedTrajectory timed_ttj = TimingUtil::createConstantTimeTrajectory(ttj, timestep);

	std::vector<std::string> headers = { "time", "x", "y", "heading", "curvature", "dscurvature", "position", "velocity", "acceleration" };
	CSVWriter::write< std::vector<TimedTrajectoryPoint>::const_iterator>(name, headers, timed_ttj.getPoints().begin(), timed_ttj.getPoints().end());
}