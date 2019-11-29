// RobotGen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#ifdef __GNUC__
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif


std::string robotsdir;
std::string newname;

std::string sub1 = "robotname";
std::string sub2 = "RobotName";
std::string sub3 = "template";
std::string sub4 = "$$robotname$$";
std::string sub5 = "$$RobotName$$";

std::string replace(const std::string& str, const std::string &newname, bool sim)
{
	std::string ret = str;
	std::string lower = newname;

	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });

	while (true)
	{
		size_t start = ret.find(sub1);
		if (start == std::string::npos)
			break;

		ret = ret.substr(0, start) + lower + ret.substr(start + sub1.length());
	}

	while (true)
	{
		size_t start = ret.find(sub2);
		if (start == std::string::npos)
			break;

		ret = ret.substr(0, start) + newname + ret.substr(start + sub2.length());
	}

	while (true)
	{
		size_t start = ret.find(sub3);
		if (start == std::string::npos)
			break;

		if (sim)
		{
			ret = ret.substr(0, start) + lower + "sim" + ret.substr(start + sub3.length());
		}
		else
		{
			ret = ret.substr(0, start) + lower + ret.substr(start + sub3.length());
		}
	}

	return ret;
}

void copyfile(fs::path srcfile, fs::path destfile, const std::string &newname)
{
	std::ofstream outfile(destfile);
	std::ifstream infile(srcfile);
	std::string line;
	std::string lower = newname;

	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });

	while (std::getline(infile, line))
	{
		while (true)
		{
			size_t start = line.find(sub4);
			if (start == std::string::npos)
				break;

			line = line.substr(0, start) + lower + line.substr(start + sub4.length());
		}

		while (true)
		{
			size_t start = line.find(sub5);
			if (start == std::string::npos)
				break;

			line = line.substr(0, start) + newname + line.substr(start + sub5.length());
		}

		outfile << line << std::endl;
	}
}

void copyfiles(fs::path src, fs::path dest, const std::string &newname, bool sim)
{
	if (!fs::exists(dest))
		fs::create_directory(dest);

	for (auto& p : fs::directory_iterator(src))
	{
		std::cout << "Processing file '" << p.path().string() << "'" << std::endl;
		if (fs::is_directory(p.path()))
		{
			fs::path newsrc = src / p.path().filename();

			fs::path newdest = dest / p.path().filename();
			std::string deststr = replace(newdest.string(), newname, sim);
			newdest = fs::path(deststr);

			copyfiles(newsrc, newdest, newname, sim);
		}
		else
		{
			std::string destname = replace(p.path().string(), newname, sim);
			copyfile(p.path(), fs::path(destname), newname);
		}
	}
}

int main(int ac, char **av)
{
	ac--;
	av++;

	if (ac != 1)
	{
		std::cerr << "robotgen: invalid number of arguments - expected one, the name of the new robot" << std::endl;
		return 1;
	}

	newname = *av++;

	std::string lower = newname;
	std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return std::tolower(c); });

	fs::path rdir = "robots";
	fs::path robottemplatepath = rdir / "template";
	fs::path robotnewpath = rdir / lower;

	fs::path simdir = "robotsims";
	fs::path simtemplatepath = simdir / "template";
	fs::path newsimpath = simdir / (lower + "sim");

	if (fs::exists(robotnewpath)) {
		std::cerr << "robotgen: new robot directory '" << robotnewpath << "' already exists - program aborted" << std::endl;
		return 1;
	}

	if (fs::exists(newsimpath)) {
		std::cerr << "robotgen: new robot simulator directory '" << newsimpath << "' already exists - program aborted" << std::endl;
		return 1;
	}

	if (!fs::create_directory(robotnewpath)) {
		std::cerr << "robotgen: cannot create directory '" << robotnewpath << "' for new robot - program aborted" << std::endl;
		return 1;
	}

	if (!fs::create_directory(newsimpath)) {
		std::cerr << "robotgen: cannot create directory '" << newsimpath << "' for new robot - program aborted" << std::endl;
		return 1;
	}

	copyfiles(robottemplatepath, robotnewpath, newname, false);
	copyfiles(simtemplatepath, newsimpath, newname, true);

	return 0;
}
