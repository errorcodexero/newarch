// DevApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "JSONPathReader.h"
#include "PathCollection.h"
#include "CSVWriter.h"
#include "Trajectory.h"
#include "PathGenerator.h"
#include "pathfinder.h"
#include <iostream>

using namespace xero::paths;

std::string pathfile;
std::string outputdir;
std::string outfile;
int step = PATHFINDER_SAMPLES_LOW;
double timestep = 0.02;
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

		if (arg == "--step")
		{
			if (ac == 0) {
				std::cerr << "pathgen: expected floating point nubmer following --step argument" << std::endl;
				return 1;
			}

			arg = *av;
			try {
				step = std::stoi(arg, &index);
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

void generateForGroup(const std::string & group)
{
	auto gptr = collection.findGroupByName(group);
	if (gptr == nullptr)
	{
		std::cerr << "pathgen: warning: path group '" << group << "' does not exist" << std::endl;
		return;
	}

	std::cout << "Generating paths for group " << group << " ... " << std::endl;
	std::vector<std::string> paths = gptr->getPathNames();
	for (const std::string& path : paths)
		generateForPath(*gptr, path);
}

void generateForPath(PathGroup & group, const std::string & path)
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
	PathGenerator gen(step, timestep);
	TimedTrajectory trajectory = gen.generate(pptr->getPoints(), pptr->getMaxVelocity(), pptr->getMaxVelocity(), pptr->getMaxJerk());

	std::string name;
	if (outputdir.length())
		name = outputdir + "/" + group.getName() + "_" + path + "_path.csv";
	else
		name = outfile;

	std::vector<std::string> headers = { "time", "x", "y", "heading", "curvature", "dscurvature", "position", "velocity", "acceleration" };
	CSVWriter::write< std::vector<TimedTrajectoryPoint>::const_iterator>(name, headers, trajectory.getPoints().begin(), trajectory.getPoints().end());
}
